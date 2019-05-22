#include <stdint.h>
#include <string.h>
#include "track.h"
#include "typehelp.h"


enum meta_event {
    META_TRACK_END = 0x2F,
    META_TEMPO = 0x51,
    META_TIME_SIGNATURE = 0x58,
    META_KEY_SIGNATURE = 0x59,
};


static uint32_t track_copy_bytes(char *track, char *bytes, uint32_t num_bytes) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    char *p = &track[8 + curr_bytes.value];
    memcpy(p, bytes, num_bytes);
    *(fourbytes *)&track[4] = flip4(curr_bytes.value + num_bytes);
    return num_bytes;
}


static char *add_delta(char *p, uint32_t delta) {
    unsigned char *bytes = (char *)&delta;

    bytes[3] = 0x7F & ((bytes[3] << 3) | (bytes[2] >> 5));
    bytes[2] = 0x7F & ((bytes[2] << 2) | (bytes[1] >> 6));
    bytes[1] = 0x7F & ((bytes[1] << 1) | (bytes[0] >> 7));
    bytes[0] = 0x7F & bytes[0];

    if(bytes[3])
        *p++ = 0x80 | bytes[3];
    if(bytes[3] || bytes[2])
        *p++ = 0x80 | bytes[2];
    if(bytes[3] || bytes[2] || bytes[1])
        *p++ = 0x80 | bytes[1];
    *p++ = bytes[0];

    return p;
}


uint32_t track_init(char *out) {
    memcpy(&out[0], "MTrk", 4);
    *(fourbytes *)&out[4] = flip4(0);
    return 8;
}


uint32_t track_tempo(char *track, uint32_t delta, uint32_t quart_micros) {
    char bytes[10];
    char *p = add_delta(bytes, delta);
    *p++ = 0xFF;
    *p++ = META_TEMPO;
    *p++ = 0x03;
    *(threebytes *)p = flip3lower(quart_micros);
    p += 3;
    return track_copy_bytes(track, bytes, p - bytes);
}


uint32_t track_time_signature(char *track, uint32_t delta, char num, char denomexp) {
    char bytes[11];
    char *p = add_delta(bytes, delta);
    *p++ = 0xFF;
    *p++ = META_TIME_SIGNATURE;
    *p++ = 0x04;
    *p++ = num;
    *p++ = denomexp;
    *p++ = 24;
    *p++ = 8;
    return track_copy_bytes(track, bytes, p - bytes);
}


uint32_t track_key(char *track, uint32_t delta, twobytes key) {
    char bytes[9];
    char *p = add_delta(bytes, delta);
    *p++ = 0xFF;
    *p++ = META_KEY_SIGNATURE;
    *p++ = 0x02;
    memcpy(p, &key, 2);
    p += 2;
    return track_copy_bytes(track, bytes, p - bytes);
}


uint32_t track_program_no(char *track, uint32_t delta, char channel, char program_no) {
    char new_bytes[6];

    char *p = add_delta(new_bytes, delta); /* at most 4 bytes */
    *p++ = 0xC0 | channel; /* 0xC0 is program number status (status is or-ed with channel) */
    *p++ = program_no;

    return track_copy_bytes(track, new_bytes, p - new_bytes);
}


uint32_t track_note_on(char *track, uint32_t delta, char channel, char pitch, char velocity) {
    char new_bytes[7];

    char *p = add_delta(new_bytes, delta); /* at most 4 bytes */
    *p++ = 0x90 | channel; /* 0x90 = note-on status (status is or-ed with channel) */
    *p++ = pitch;
    *p++ = velocity;

    return track_copy_bytes(track, new_bytes, p - new_bytes);
}


uint32_t track_end(char *track, uint32_t delta) {
    char bytes[7];

    char *p = add_delta(bytes, delta); /* at most 4 bytes */
    *p++ = 0xFF; /* "meta" chunk */
    *p++ = META_TRACK_END;
    *p++ = 0x00; /* no more bytes */

    return track_copy_bytes(track, bytes, p - bytes);
}
