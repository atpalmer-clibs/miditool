#ifndef TRACK_H
#define TRACK_H

#include <stdint.h>
#include "typehelp.h"

uint32_t track_init(char *out);
uint32_t track_time_signature(char *track, char num, char denomexp);
uint32_t track_tempo(char *track, uint32_t quarter_micros);
uint32_t track_key(char *track, twobytes key);
uint32_t track_program_no(char *track, char delta, char channel, char program_no);
uint32_t track_midi_event(char *track, char delta, char status, char channel, char pitch, char velocity);
uint32_t track_end(char *track);

#endif
