#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "track.h"
#include "typehelp.h"
#include "bytebuff.h"


uint32_t fill_header(struct bytebuff *buff, uint16_t format, uint16_t tracks, uint16_t division) {
    bytebuff_append_string(buff, "MThd");
    bytebuff_append_uint32(buff, 6);        /* num bytes following in header */
    bytebuff_append_uint16(buff, format);   /* 0=single-track; 1=multi-track; 2=multi-song */
    bytebuff_append_uint16(buff, tracks);   /* number of tracks following header */
    bytebuff_append_uint16(buff, division); /* +=ticks per beat; -=SMPTE units */
    return 14;
}


int main(void) {
    struct bytebuff *bytebuff = bytebuff_new();
    uint8_t *buff = bytebuff->bytes;

    fill_header(bytebuff, FORMAT_SINGLE_TRACK, 1, DIVISION_TICKS_PER_BEAT(10000));

    MidiTrack *trackobj = track_start(bytebuff);
    track_tempo(trackobj, 0, BPM_TO_MICROS(60));
    track_key(trackobj, 0, KEY_C_MAJOR);
    track_time_signature(trackobj, 0, 2, TIMESIG_DENOM_4);
    track_program_no(trackobj, 0, CHANNEL(1), 46);
    track_note_on(trackobj, 0, CHANNEL(1), PITCH_G3, VELOCITY_MAX);
    track_note_on(trackobj, 0xFFFF, CHANNEL(1), PITCH_C4, VELOCITY_MAX);
    track_note_on(trackobj, 0xFFFF, CHANNEL(1), PITCH_E4, VELOCITY_MAX);
    track_note_on(trackobj, 0xFFFF, CHANNEL(1), PITCH_G4, VELOCITY_MAX);
    track_note_on(trackobj, 0xFFFF, CHANNEL(1), PITCH_B4, VELOCITY_MAX);
    track_end(trackobj, 0);

    uint32_t bytesused = bytebuff->p - bytebuff->bytes;

    FILE *f = fopen("out.mid", "wb");
    fwrite(buff, bytesused, 1, f);
    fclose(f);

    track_free(trackobj);
    bytebuff_free(bytebuff);
    printf("Done\n");
}
