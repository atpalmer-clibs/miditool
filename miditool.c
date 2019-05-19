#include <stdint.h>
#include <stdio.h>
#include <string.h>

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


typedef struct twobytes {
    char bytes[2];
} twobytes;


typedef union fourbytes {
    char bytes[4];
    uint32_t value;
} fourbytes;


twobytes flip2(uint16_t in) {
    char *bytes = (char *)&in;
    twobytes result;
    result.bytes[0] = bytes[1]; 
    result.bytes[1] = bytes[0]; 
    return result;
}


fourbytes flip4(uint32_t in) {
    char *bytes = (char *)&in;
    fourbytes result;
    result.bytes[0] = bytes[3]; 
    result.bytes[1] = bytes[2]; 
    result.bytes[2] = bytes[1]; 
    result.bytes[3] = bytes[0]; 
    return result;
}


size_t fill_header(char *out, uint16_t format, uint16_t tracks, uint16_t division) {
    memcpy(&out[0], MIDI_HEADER, 4) ;
    *(fourbytes *)&out[4] = flip4(6);
    *(twobytes *)&out[8] = flip2(format);
    *(twobytes *)&out[10] = flip2(tracks);
    *(twobytes *)&out[12] = flip2(division);
    return 14;
}


size_t track_midi_event(char *track, char delta, char status, char channel, char pitch, char velocity) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    char *p = &track[8 + curr_bytes.value];

    uint32_t new_bytes = 0;
    if(delta)
        p[new_bytes++] = delta;
    p[new_bytes++] = 0;
    p[new_bytes++] = status | channel;
    p[new_bytes++] = pitch;
    p[new_bytes++] = velocity;

    *(fourbytes *)&track[4] = flip4(curr_bytes.value + new_bytes); /* update track size */

    return new_bytes;
}


size_t track_init(char *out) {
    memcpy(&out[0], MIDI_TRACK, 4);
    *(fourbytes *)&out[4] = flip4(0);
    return 8;
}


size_t track_end(char *track) {
    fourbytes curr_bytes = flip4(*(uint32_t *)&track[4]);
    char *p = &track[8 + curr_bytes.value];

    p[0] = 0xFF;
    p[1] = 0x2F;
    p[2] = 0x00;

    *(fourbytes *)&track[4] = flip4(curr_bytes.value + 3); /* update track size */

    return 3;
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
