#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "track.h"
#include "typehelp.h"
#include "bytebuff.h"

enum status {
    /* MIDI events: or-ed with channel */
    STATUS_PROGRAM_NO = 0xC0,
    STATUS_NOTE_ON = 0x90,
    STATUS_NOTE_OFF = 0x80,

    /* Meta events */
    STATUS_META_CHUNK = 0xFF,
};

enum meta_event {
    META_TRACK_END = 0x2F,
    META_TEMPO = 0x51,
    META_TIME_SIGNATURE = 0x58,
    META_KEY_SIGNATURE = 0x59,
};


#define TRACK_SIZE_RAW(this)  (&(&(this)->buff->bytes[(this)->head])[4])


static void add_uint32_value_at_raw(void *raw, uint32_t value) {
    /* add little-endian value to big-endian "raw" pointer */

    uint32_t current = flip4(*(uint32_t *)raw).value;
    uint32_t new = current + value;
    memcpy(raw, flip4(new).bytes, 4);
}


static void track_append_bytes(MidiTrack *this, void *bytes, uint32_t num_bytes) {
    bytebuff_append_raw(this->buff, bytes, num_bytes);
    add_uint32_value_at_raw(TRACK_SIZE_RAW(this), num_bytes);
}


static uint8_t *add_delta(uint8_t *p, uint32_t delta) {
    /* MIDI delta-time uses variable-width format.
     * Top bit of each byte indicates more bytes.
     * Never more than four bytes.
     */
    uint8_t *bytes = flip4(delta).bytes;

    bytes[0] = 0x7F & ((bytes[0] << 3) | (bytes[1] >> 5));
    bytes[1] = 0x7F & ((bytes[1] << 2) | (bytes[2] >> 6));
    bytes[2] = 0x7F & ((bytes[2] << 1) | (bytes[3] >> 7));
    bytes[3] = 0x7F & bytes[3];

    if(bytes[0])
        *p++ = 0x80 | bytes[0];
    if(bytes[0] || bytes[1])
        *p++ = 0x80 | bytes[1];
    if(bytes[0] || bytes[1] || bytes[2])
        *p++ = 0x80 | bytes[2];
    *p++ = 0x7F & bytes[3];

    return p;
}


MidiTrack *track_start(MidiBuffer *buff) {
    MidiTrack *new = malloc(sizeof *new);
    new->buff = buff;
    new->head = buff->used;
    bytebuff_append_string(buff, "MTrk");
    bytebuff_append_uint32(buff, 0);
    return new;
}


void track_free(MidiTrack *this) {
    free(this);
}


void track_tempo(MidiTrack *this, uint32_t delta, uint32_t quart_micros) {
    uint8_t bytes[10];
    uint8_t *p = add_delta(bytes, delta);
    *p++ = STATUS_META_CHUNK;
    *p++ = META_TEMPO;
    *p++ = 3; /* bytes following... */
    *(threebytes *)p = flip3lower(quart_micros);
    p += 3;
    track_append_bytes(this, bytes, p - bytes);
}


void track_key(MidiTrack *this, uint32_t delta, uint16_t key) {
    uint8_t bytes[9];
    uint8_t *p = add_delta(bytes, delta);
    *p++ = STATUS_META_CHUNK;
    *p++ = META_KEY_SIGNATURE;
    *p++ = 2; /* bytes following... */
    *(uint16_t *)p = key;
    p += 2;
    track_append_bytes(this, bytes, p - bytes);
}


void track_time_signature(MidiTrack *this, uint32_t delta, uint8_t num, uint8_t denomexp) {
    uint8_t bytes[11];
    uint8_t *p = add_delta(bytes, delta);
    *p++ = STATUS_META_CHUNK;
    *p++ = META_TIME_SIGNATURE;
    *p++ = 4; /* bytes following... */
    *p++ = num;
    *p++ = denomexp;
    *p++ = 24;
    *p++ = 8;
    track_append_bytes(this, bytes, p - bytes);
}


void track_program_no(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t program_no) {
    uint8_t new_bytes[6];
    uint8_t *p = add_delta(new_bytes, delta); /* at most 4 bytes */
    *p++ = STATUS_PROGRAM_NO | channel;
    *p++ = program_no;
    track_append_bytes(this, new_bytes, p - new_bytes);
}


void track_note_on(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity) {
    uint8_t new_bytes[7];
    uint8_t *p = add_delta(new_bytes, delta); /* at most 4 bytes */
    *p++ = STATUS_NOTE_ON | channel;
    *p++ = pitch;
    *p++ = velocity;
    track_append_bytes(this, new_bytes, p - new_bytes);
}


void track_note_off(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity) {
    uint8_t new_bytes[7];
    uint8_t *p = add_delta(new_bytes, delta); /* at most 4 bytes */
    *p++ = STATUS_NOTE_OFF | channel;
    *p++ = pitch;
    *p++ = velocity;
    track_append_bytes(this, new_bytes, p - new_bytes);
}


void track_end(MidiTrack *this, uint32_t delta) {
    uint8_t bytes[7];
    uint8_t *p = add_delta(bytes, delta); /* at most 4 bytes */
    *p++ = STATUS_META_CHUNK;
    *p++ = META_TRACK_END;
    *p++ = 0; /* no more bytes */
    track_append_bytes(this, bytes, p - bytes);
}

