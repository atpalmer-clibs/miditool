#ifndef TRACK_H
#define TRACK_H

#include <stdint.h>
#include "bytebuff.h"

typedef struct track {
    struct bytebuff *buff;
    size_t head;
} MidiTrack;

MidiTrack *track_start(struct bytebuff *buff);
void track_free(MidiTrack *this);
uint32_t track_tempo(MidiTrack *this, uint32_t delta, uint32_t quarter_micros);
uint32_t track_key(MidiTrack *this, uint32_t delta, uint16_t key);

uint32_t track_time_signature(uint8_t *track, uint32_t delta, uint8_t num, uint8_t denomexp);
uint32_t track_program_no(uint8_t *track, uint32_t delta, uint8_t channel, uint8_t program_no);
uint32_t track_note_on(uint8_t *track, uint32_t delta, uint8_t channel, uint8_t pitch, uint8_t velocity);
uint32_t track_end(uint8_t *track, uint32_t delta);

#endif
