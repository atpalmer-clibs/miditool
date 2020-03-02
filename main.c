#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "track.h"
#include "midibuff.h"


int main(void) {
    MidiBuffer *buff = midibuff_new(FORMAT_SINGLE_TRACK, 1, DIVISION_TICKS_PER_BEAT(10000));

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
