#include <stdint.h>
#include <string.h>
#include "track.h"
#include "typehelp.h"


static char *track_pointer(char *track) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    char *p = &track[8 + curr_bytes.value];
    return p;
}


static uint32_t track_update(char *track, uint32_t new_bytes) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    *(fourbytes *)&track[4] = flip4(curr_bytes.value + new_bytes);
    return new_bytes;
}


uint32_t track_init(char *out) {
    memcpy(&out[0], "MTrk", 4);
    *(fourbytes *)&out[4] = flip4(0);
    return 8;
}


uint32_t track_midi_event(char *track, char delta, char status, char channel, char pitch, char velocity) {
    char *p = track_pointer(track);

    uint32_t new_bytes = 0;
    if(delta)
        p[new_bytes++] = delta;
    p[new_bytes++] = 0;
    p[new_bytes++] = status | channel;
    p[new_bytes++] = pitch;
    p[new_bytes++] = velocity;

    return track_update(track, new_bytes);
}


uint32_t track_end(char *track) {
    char *p = track_pointer(track);

    p[0] = 0xFF;
    p[1] = 0x2F;
    p[2] = 0x00;

    return track_update(track, 3);
}
