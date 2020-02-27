#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "track.h"
#include "typehelp.h"

#define BPM_TO_MICROS(bpm) ((uint32_t)((60.0 * 1000000.0)/(double)bpm))

enum pitch {
    PITCH_G3 = 0x37,
    PITCH_C4 = 0x3C,
    PITCH_Db4 = 0x3D,
    PITCH_D4 = 0x3E,
    PITCH_Eb4 = 0x3F,
    PITCH_E4 = 0x40,
    PITCH_F4 = 0x41,
    PITCH_Gb4 = 0x42,
    PITCH_G4 = 0x43,
    PITCH_Ab4 = 0x44,
    PITCH_A4 = 0x45,
    PITCH_Bb4 = 0x46,
    PITCH_B4 = 0x47,
    PITCH_C5 = 0x48,
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

enum velocity {
    VELOCITY_MAX = 0x7F,
};

uint32_t fill_header(uint8_t *out, uint16_t format, uint16_t tracks, uint16_t division) {
    memcpy(&out[0], "MThd", 4) ;
    *(fourbytes *)&out[4] = flip4(6);
    *(twobytes *)&out[8] = flip2(format);
    *(twobytes *)&out[10] = flip2(tracks);
    *(twobytes *)&out[12] = flip2(division);
    return 14;
}


int main(void) {
    uint32_t bytesused = 0;
    uint8_t buff[1024] = {0};

    bytesused += fill_header(&buff[bytesused], 0, 1, 10000);

    uint8_t *track = &buff[bytesused];
    bytesused += track_init(track);
    bytesused += track_tempo(track, 0, BPM_TO_MICROS(60));
    bytesused += track_key(track, 0, KEY_C_MAJOR);
    bytesused += track_time_signature(track, 0, 2, TIMESIG_DENOM_4);
    bytesused += track_program_no(track, 0, 0, 46);
    bytesused += track_note_on(track, 0, 0, PITCH_G3, VELOCITY_MAX);
    bytesused += track_note_on(track, 0xFF, 0, PITCH_C4, VELOCITY_MAX);
    bytesused += track_note_on(track, 0xFF, 0, PITCH_E4, VELOCITY_MAX);
    bytesused += track_note_on(track, 0xFF, 0, PITCH_G4, VELOCITY_MAX);
    bytesused += track_note_on(track, 0xFF, 0, PITCH_B4, VELOCITY_MAX);
    bytesused += track_end(track, 0);

    FILE *f = fopen("out.mid", "wb");
    fwrite(&buff, bytesused, 1, f);
    fclose(f);

    printf("Done\n");
}
