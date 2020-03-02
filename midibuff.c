#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "midibuff.h"
#include "typehelp.h"

#define NEXT_BYTE(this) ((this)->bytes[(this)->used])

const int INITIAL_CAP = 1024;

MidiBuffer *bytebuff_new(void) {
    MidiBuffer *new = malloc(sizeof *new);
    new->bytes = malloc(INITIAL_CAP);
    new->used = 0;
    new->cap = INITIAL_CAP;
    memset(new->bytes, 0, INITIAL_CAP);
    return new;
}

void bytebuff_free(MidiBuffer *this) {
    free(this->bytes);
    free(this);
}

void bytebuff_append_raw(MidiBuffer *this, void *data, size_t count) {
    assert(this->used < this->cap);
    memcpy(&NEXT_BYTE(this), data, count);
    this->used += count;
}

void bytebuff_append_string(MidiBuffer *this, char *data) {
    size_t count = strlen(data);
    bytebuff_append_raw(this, data, count);
}

void bytebuff_append_uint32(MidiBuffer *this, uint32_t value) {
    uint8_t *data = flip4(value).bytes;
    bytebuff_append_raw(this, data, 4);
}

void bytebuff_append_uint16(MidiBuffer *this, uint16_t value) {
    uint8_t *data = flip2(value).bytes;
    bytebuff_append_raw(this, data, 2);
}

void bytebuff_append_uint8(MidiBuffer *this, uint8_t value) {
    uint8_t *data = &value;
    bytebuff_append_raw(this, data, 1);
}

