#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "track.h"
#include "typehelp.h"
#include "midibuff.h"

#define ARRSIZE(arr) (sizeof(arr)/sizeof(*arr))

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
    midibuff_append_raw(this->buff, bytes, num_bytes);
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


static void track_append_chunk(MidiTrack *this, uint32_t delta, void *chunk, size_t chunk_size) {
    uint8_t new_bytes[4 + chunk_size];
    uint8_t *p = add_delta(new_bytes, delta);
    size_t total_size = (p - new_bytes) + chunk_size;

    memcpy(&new_bytes[p - new_bytes], chunk, chunk_size);

    track_append_bytes(this, new_bytes, total_size);
}


MidiTrack *track_start(MidiBuffer *buff) {
    MidiTrack *new = malloc(sizeof *new);
    new->buff = buff;
    new->head = buff->used;
    midibuff_append_string(buff, "MTrk");
    midibuff_append_uint32(buff, 0);
    return new;
}


void track_free(MidiTrack *this) {
    free(this);
}


void track_tempo(MidiTrack *this, uint32_t delta, uint32_t quart_micros) {
    uint8_t *quart_micros_as_raw = flip3lower(quart_micros).bytes;

    uint8_t chunk[] = {
        STATUS_META_CHUNK,
        META_TEMPO,
        3, /* bytes following... */
        quart_micros_as_raw[0],
        quart_micros_as_raw[1],
        quart_micros_as_raw[2],
    };

    track_append_chunk(this, delta, chunk, ARRSIZE(chunk));
}


void track_key(MidiTrack *this, uint32_t delta, uint16_t key) {
    uint8_t chunk[] = {
        STATUS_META_CHUNK,
        META_KEY_SIGNATURE,
        2, /* bytes following... */
        (&key)[0],
        (&key)[1],
    };

    track_append_chunk(this, delta, chunk, ARRSIZE(chunk));
}


void track_time_signature(MidiTrack *this, uint32_t delta, uint8_t num, uint8_t denomexp) {
    uint8_t chunk[] = {
        STATUS_META_CHUNK,
        META_TIME_SIGNATURE,
        4, /* bytes following... */
        num,
        denomexp,
        24,
        8,
    };

    track_append_chunk(this, delta, chunk, ARRSIZE(chunk));
}


void track_program_no(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t program_no) {
    uint8_t chunk[] = {
        STATUS_PROGRAM_NO | channel,
        program_no,
    };

    track_append_chunk(this, delta, chunk, ARRSIZE(chunk));
}


void track_note_on(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity) {
    uint8_t chunk[] = {
        STATUS_NOTE_ON | channel,
        pitch,
        velocity,
    };

    track_append_chunk(this, delta, chunk, ARRSIZE(chunk));
}


void track_note_off(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity) {
    uint8_t chunk[] = {
        STATUS_NOTE_OFF | channel,
        pitch,
        velocity,
    };

    track_append_chunk(this, delta, chunk, ARRSIZE(chunk));
}


void track_end(MidiTrack *this, uint32_t delta) {
    uint8_t chunk[] = {
        STATUS_META_CHUNK,
        META_TRACK_END,
        0, /* no more bytes */
    };

    track_append_chunk(this, delta, chunk, ARRSIZE(chunk));
}

