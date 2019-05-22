#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "track.h"
#include "typehelp.h"

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


const twobytes KEY_G_MAJOR = { 0x01, 0x00 };
const twobytes KEY_C_MAJOR = { 0x00, 0x00 };
const twobytes KEY_F_MAJOR = { 0x81, 0x00 };

enum timesig_denom {
    TIMESIG_DENOM_1 = 0,
    TIMESIG_DENOM_2 = 1,
    TIMESIG_DENOM_4 = 2,
    TIMESIG_DENOM_8 = 3,
    TIMESIG_DENOM_16 = 4,
    TIMESIG_DENOM_32 = 5,
};

uint32_t fill_header(char *out, uint16_t format, uint16_t tracks, uint16_t division) {
    memcpy(&out[0], "MThd", 4) ;
    *(fourbytes *)&out[4] = flip4(6);
    *(twobytes *)&out[8] = flip2(format);
    *(twobytes *)&out[10] = flip2(tracks);
    *(twobytes *)&out[12] = flip2(division);
    return 14;
}


int main(int argc, char **argv) {
    uint32_t bytesused = 0;
    char buff[1024] = {0};

    bytesused += fill_header(&buff[bytesused], 0, 1, 10000);

    char *track = &buff[bytesused];
    bytesused += track_init(track);
    bytesused += track_tempo(track, 0, 1000000000);
    bytesused += track_key(track, 0, KEY_C_MAJOR);
    bytesused += track_time_signature(track, 0, 2, TIMESIG_DENOM_4);
    bytesused += track_program_no(track, 0, 0, 5);
    bytesused += track_note_on(track, 0, 0, PITCH_C, 0x7F);
    bytesused += track_note_on(track, 0xFF, 0, PITCH_E, 0x7F);
    bytesused += track_note_on(track, 0xFF, 0, PITCH_G, 0x7F);
    bytesused += track_note_on(track, 0xFF, 0, PITCH_B, 0x7F);
    bytesused += track_end(track, 0);

    FILE *f = fopen("out.mid", "wb");
    fwrite(&buff, bytesused, 1, f);
    fclose(f);

    printf("Done\n");
}
