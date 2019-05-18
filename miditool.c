#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char MIDI_HEADER[4] = "MThd";
const char MIDI_TRACK[4] = "MTrk";


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


int main(int argc, char **argv) {
    size_t bytesused = 0;
    char buff[1024] = {0};

    bytesused += fill_header(&buff[0], utotwobytes(0), utotwobytes(0), utotwobytes(0));

    FILE *f = fopen("out.mid", "wb");
    fwrite(&buff, bytesused, 1, f);
    fclose(f);

    printf("Done\n");
}
