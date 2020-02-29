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


static uint32_t track_copy_bytes(uint8_t *track, uint8_t *bytes, uint32_t num_bytes) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    uint8_t *p = &track[8 + curr_bytes.value];
    memcpy(p, bytes, num_bytes);
    *(fourbytes *)&track[4] = flip4(curr_bytes.value + num_bytes);
    return num_bytes;
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


MidiTrack *track_start(struct bytebuff *buff) {
    MidiTrack *new = malloc(sizeof *new);
    new->buff = buff;
    new->head = buff->p - buff->bytes;
    bytebuff_append_string(buff, "MTrk");
    bytebuff_append_uint32(buff, 0);
    return new;
}


void track_free(MidiTrack *this) {
    free(this);
}


uint32_t track_tempo(MidiTrack *this, uint32_t delta, uint32_t quart_micros) {
    void *track = &this->buff->bytes[this->head];

    uint8_t bytes[10];
    uint8_t *p = add_delta(bytes, delta);
    *p++ = STATUS_META_CHUNK;
    *p++ = META_TEMPO;
    *p++ = 3; /* bytes following... */
    *(threebytes *)p = flip3lower(quart_micros);
    p += 3;
    return track_copy_bytes(track, bytes, p - bytes);
}


uint32_t track_key(MidiTrack *this, uint32_t delta, uint16_t key) {
    void *track = &this->buff->bytes[this->head];

    uint8_t bytes[9];
    uint8_t *p = add_delta(bytes, delta);
    *p++ = STATUS_META_CHUNK;
    *p++ = META_KEY_SIGNATURE;
    *p++ = 2; /* bytes following... */
    *(uint16_t *)p = key;
    p += 2;
    return track_copy_bytes(track, bytes, p - bytes);
}


uint32_t track_time_signature(MidiTrack *this, uint32_t delta, uint8_t num, uint8_t denomexp) {
    void *track = &this->buff->bytes[this->head];

    uint8_t bytes[11];
    uint8_t *p = add_delta(bytes, delta);
    *p++ = STATUS_META_CHUNK;
    *p++ = META_TIME_SIGNATURE;
    *p++ = 4; /* bytes following... */
    *p++ = num;
    *p++ = denomexp;
    *p++ = 24;
    *p++ = 8;
    return track_copy_bytes(track, bytes, p - bytes);
}


uint32_t track_program_no(uint8_t *track, uint32_t delta, uint8_t channel, uint8_t program_no) {
    uint8_t new_bytes[6];

    uint8_t *p = add_delta(new_bytes, delta); /* at most 4 bytes */
    *p++ = STATUS_PROGRAM_NO | channel;
    *p++ = program_no;

    return track_copy_bytes(track, new_bytes, p - new_bytes);
}

uint32_t track_note_on(uint8_t *track, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity) {
    uint8_t new_bytes[7];

    uint8_t *p = add_delta(new_bytes, delta); /* at most 4 bytes */
    *p++ = STATUS_NOTE_ON | channel;
    *p++ = pitch;
    *p++ = velocity;

    return track_copy_bytes(track, new_bytes, p - new_bytes);
}


uint32_t track_note_off(uint8_t *track, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity) {
    uint8_t new_bytes[7];

    uint8_t *p = add_delta(new_bytes, delta); /* at most 4 bytes */
    *p++ = STATUS_NOTE_OFF | channel;
    *p++ = pitch;
    *p++ = velocity;

    return track_copy_bytes(track, new_bytes, p - new_bytes);
}


uint32_t track_end(uint8_t *track, uint32_t delta) {
    uint8_t bytes[7];

    uint8_t *p = add_delta(bytes, delta); /* at most 4 bytes */
    *p++ = STATUS_META_CHUNK;
    *p++ = META_TRACK_END;
    *p++ = 0; /* no more bytes */

    return track_copy_bytes(track, bytes, p - bytes);
}
