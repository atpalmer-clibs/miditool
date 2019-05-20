#include <stdint.h>
#include <string.h>
#include "track.h"
#include "typehelp.h"


static uint32_t track_copy_bytes(char *track, char *bytes, uint32_t num_bytes) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    char *p = &track[8 + curr_bytes.value];
    memcpy(p, bytes, num_bytes);
    *(fourbytes *)&track[4] = flip4(curr_bytes.value + num_bytes);
    return num_bytes;
}


uint32_t track_init(char *out) {
    memcpy(&out[0], "MTrk", 4);
    *(fourbytes *)&out[4] = flip4(0);
    return 8;
}


uint32_t track_tempo(char *track, uint32_t quart_micros) {
    char bytes[6] = { 0xFF, 0x51, 0x03 };
    *(threebytes *)&bytes[3] = flip3lower(quart_micros);
    return track_copy_bytes(track, bytes, 6);
}


uint32_t track_midi_event(char *track, char delta, char status, char channel, char pitch, char velocity) {
    char new_bytes[1024];
    char *p = new_bytes;

    if(delta)
        *p++ = delta;
    *p++ = 0x00; /* 0-byte ends delta-time */
    *p++ = status | channel;
    *p++ = pitch;
    *p++ = velocity;

    return track_copy_bytes(track, new_bytes, p - new_bytes);
}


uint32_t track_end(char *track) {
    char bytes[3] = { 0xFF, 0x2F, 0x00 };
    return track_copy_bytes(track, bytes, 3);
}
