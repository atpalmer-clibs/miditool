#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char MIDI_HEADER[4] = "MThd";
const char MIDI_TRACK[4] = "MTrk";

enum status {
    STATUS_NOTEON = 0x09 << 4,
};

enum pitch {
    PITCH_C = 0x3C,
};


typedef struct twobytes {
    char bytes[2];
} twobytes;


typedef struct fourbytes {
    char bytes[4];
} fourbytes;


twobytes utotwobytes(uint16_t in) {
    char *bytes = (char *)&in;
    twobytes result;
    result.bytes[0] = bytes[1]; 
    result.bytes[1] = bytes[0]; 
    return result;
}


fourbytes utofourbytes(uint32_t in) {
    char *bytes = (char *)&in;
    fourbytes result;
    result.bytes[0] = bytes[3]; 
    result.bytes[1] = bytes[2]; 
    result.bytes[2] = bytes[1]; 
    result.bytes[3] = bytes[0]; 
    return result;
}


size_t fill_header(char *out, twobytes format, twobytes tracks, twobytes division) {
    const fourbytes size = utofourbytes(6);
    memcpy(&out[0], MIDI_HEADER, 4);
    memcpy(&out[4], &size, 4);
    memcpy(&out[8], &format, 2);
    memcpy(&out[10], &tracks, 2);
    memcpy(&out[12], &division, 2);
    return 8 + 6;
}


size_t fill_midi_event(char *out, char delta, char status, char channel, char pitch, char velocity) {
    const fourbytes size = utofourbytes(4);
    memcpy(&out[0], MIDI_TRACK, 4);
    memcpy(&out[4], &size, 4);
    out[8] = delta;
    out[9] = status | channel;
    out[10] = pitch;
    out[11] = velocity;
    return 12;
}


int main(int argc, char **argv) {
    size_t bytesused = 0;
    char buff[1024] = {0};

    bytesused += fill_header(&buff[bytesused], utotwobytes(0), utotwobytes(0), utotwobytes(0));
    bytesused += fill_midi_event(&buff[bytesused], 0, STATUS_NOTEON, 0x00, PITCH_C, 0x7F);

    FILE *f = fopen("out.mid", "wb");
    fwrite(&buff, bytesused, 1, f);
    fclose(f);

    printf("Done\n");
}
