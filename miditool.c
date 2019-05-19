#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "typehelp.h"

const char MIDI_HEADER[4] = "MThd";
const char MIDI_TRACK[4] = "MTrk";

enum status {
    STATUS_NOTEOFF= 0x08 << 4,
    STATUS_NOTEON = 0x09 << 4,
};

enum pitch {
    PITCH_C = 0x3C,
    PITCH_Db = 0x3D,
    PITCH_D = 0x3E,
    PITCH_Eb = 0x3F,
    PITCH_E = 0x40,
    PITCH_F = 0x41,
    PITCH_Gb = 0x42,
    PITCH_G = 0x43,
    PITCH_Ab = 0x44,
    PITCH_A = 0x45,
    PITCH_Bb = 0x46,
    PITCH_B = 0x47,
    PITCH_C4 = 0x48,
};


uint32_t fill_header(char *out, uint16_t format, uint16_t tracks, uint16_t division) {
    memcpy(&out[0], MIDI_HEADER, 4) ;
    *(fourbytes *)&out[4] = flip4(6);
    *(twobytes *)&out[8] = flip2(format);
    *(twobytes *)&out[10] = flip2(tracks);
    *(twobytes *)&out[12] = flip2(division);
    return 14;
}


char *track_pointer(char *track) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    char *p = &track[8 + curr_bytes.value];
    return p;
}


uint32_t track_update(char *track, uint32_t new_bytes) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    *(fourbytes *)&track[4] = flip4(curr_bytes.value + new_bytes);
    return new_bytes;
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


uint32_t track_init(char *out) {
    memcpy(&out[0], MIDI_TRACK, 4);
    *(fourbytes *)&out[4] = flip4(0);
    return 8;
}


uint32_t track_end(char *track) {
    char *p = track_pointer(track);

    p[0] = 0xFF;
    p[1] = 0x2F;
    p[2] = 0x00;

    return track_update(track, 3);
}


int main(int argc, char **argv) {
    uint32_t bytesused = 0;
    char buff[1024] = {0};

    bytesused += fill_header(&buff[bytesused], 0, 1, 10000);

    char *track = &buff[bytesused];
    bytesused += track_init(track);
    bytesused += track_midi_event(track, 0, STATUS_NOTEON, 0x00, PITCH_C, 0x7F);
    bytesused += track_midi_event(track, 0, STATUS_NOTEON, 0x00, PITCH_E, 0x7F);
    bytesused += track_midi_event(track, 0, STATUS_NOTEON, 0x00, PITCH_G, 0x7F);
    bytesused += track_midi_event(track, 0, STATUS_NOTEON, 0x00, PITCH_B, 0x7F);
    bytesused += track_end(track);

    FILE *f = fopen("out.mid", "wb");
    fwrite(&buff, bytesused, 1, f);
    fclose(f);

    printf("Done\n");
}
