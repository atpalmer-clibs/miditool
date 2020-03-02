#ifndef MIDIBUFF_H
#define MIDIBUFF_H

#include <stdint.h>

typedef struct midibuff {
    uint8_t *bytes;
    size_t used;
    size_t cap;
} MidiBuffer;

MidiBuffer *bytebuff_new(void);
void bytebuff_free(MidiBuffer *this);
void bytebuff_append_raw(MidiBuffer *this, void *data, size_t count);
void bytebuff_append_string(MidiBuffer *this, char *data);
void bytebuff_append_uint32(MidiBuffer *this, uint32_t data);
void bytebuff_append_uint16(MidiBuffer *this, uint16_t data);
void bytebuff_append_uint8(MidiBuffer *this, uint8_t data);

#endif
