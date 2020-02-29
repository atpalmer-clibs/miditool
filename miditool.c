#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "track.h"
#include "typehelp.h"
#include "bytebuff.h"

#define BPM_TO_MICROS(bpm) ((uint32_t)((60.0 * 1000000.0)/(double)bpm))

#define CHANNEL(channel) ((uint8_t)(channel - 1))

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


const uint16_t KEY_G_MAJOR = 0x0100;
const uint16_t KEY_C_MAJOR = 0x0000;
const uint16_t KEY_F_MAJOR = 0x8100;

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

enum midi_format {
    FORMAT_SINGLE_TRACK = 0,
    FORMAT_MULTI_TRACK = 1,
    FORMAT_MULTI_SONG = 2,  /* i.e. series of type 0 files */
};

#define DIVISION_TICKS_PER_BEAT(ticks) (ticks)
#define DIVISION_SMPTE(value) (-1 * (value))


uint32_t fill_header(struct bytebuff *buff, uint16_t format, uint16_t tracks, uint16_t division) {
    bytebuff_append_string(buff, "MThd");
    bytebuff_append_uint32(buff, 6);        /* num bytes following in header */
    bytebuff_append_uint16(buff, format);   /* 0=single-track; 1=multi-track; 2=multi-song */
    bytebuff_append_uint16(buff, tracks);   /* number of tracks following header */
    bytebuff_append_uint16(buff, division); /* +=ticks per beat; -=SMPTE units */
    return 14;
}


int main(void) {
    uint32_t bytesused = 0;

    struct bytebuff *bytebuff = bytebuff_new();
    uint8_t *buff = bytebuff->bytes;

    bytesused += fill_header(bytebuff, FORMAT_SINGLE_TRACK, 1, DIVISION_TICKS_PER_BEAT(10000));

    MidiTrack *trackobj = track_start(bytebuff);

    uint8_t *track = &buff[bytesused];

    bytesused += 8;

    bytesused += track_tempo(track, 0, BPM_TO_MICROS(60));
    bytesused += track_key(track, 0, KEY_C_MAJOR);
    bytesused += track_time_signature(track, 0, 2, TIMESIG_DENOM_4);
    bytesused += track_program_no(track, 0, CHANNEL(1), 46);
    bytesused += track_note_on(track, 0, CHANNEL(1), PITCH_G3, VELOCITY_MAX);
    bytesused += track_note_on(track, 0xFFFF, CHANNEL(1), PITCH_C4, VELOCITY_MAX);
    bytesused += track_note_on(track, 0xFFFF, CHANNEL(1), PITCH_E4, VELOCITY_MAX);
    bytesused += track_note_on(track, 0xFFFF, CHANNEL(1), PITCH_G4, VELOCITY_MAX);
    bytesused += track_note_on(track, 0xFFFF, CHANNEL(1), PITCH_B4, VELOCITY_MAX);
    bytesused += track_end(track, 0);

    FILE *f = fopen("out.mid", "wb");
    fwrite(buff, bytesused, 1, f);
    fclose(f);

    track_free(trackobj);
    bytebuff_free(bytebuff);
    printf("Done\n");
}
