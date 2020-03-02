#ifndef TRACK_H
#define TRACK_H

#include <stdint.h>
#include "midibuff.h"

typedef struct track {
    MidiBuffer *buff;
    size_t head;
} MidiTrack;

MidiTrack *track_start(MidiBuffer *buff);
void track_free(MidiTrack *this);
void track_tempo(MidiTrack *this, uint32_t delta, uint32_t quarter_micros);
void track_key(MidiTrack *this, uint32_t delta, uint16_t key);
void track_time_signature(MidiTrack *this, uint32_t delta, uint8_t num, uint8_t denomexp);
void track_program_no(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t program_no);
void track_note_on(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity);
void track_note_off(MidiTrack *this, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity);
void track_end(MidiTrack *this, uint32_t delta);

#endif
