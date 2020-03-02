#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "track.h"
#include "typehelp.h"
#include "midibuff.h"


void fill_header(MidiBuffer *buff, uint16_t format, uint16_t tracks, uint16_t division) {
    midibuff_append_string(buff, "MThd");
    midibuff_append_uint32(buff, 6);        /* num bytes following in header */
    midibuff_append_uint16(buff, format);   /* 0=single-track; 1=multi-track; 2=multi-song */
    midibuff_append_uint16(buff, tracks);   /* number of tracks following header */
    midibuff_append_uint16(buff, division); /* +=ticks per beat; -=SMPTE units */
}


int main(void) {
    MidiBuffer *buff = midibuff_new_empty();

    fill_header(buff, FORMAT_SINGLE_TRACK, 1, DIVISION_TICKS_PER_BEAT(10000));

    MidiTrack *trackobj = track_start(buff);
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

    FILE *f = fopen("out.mid", "wb");
    fwrite(buff->bytes, buff->used, 1, f);
    fclose(f);

    track_free(trackobj);
    midibuff_free(buff);
    printf("Done\n");
}
