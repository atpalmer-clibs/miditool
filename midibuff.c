#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "midibuff.h"

#define NEXT_BYTE(this) ((this)->bytes[(this)->used])

/* TODO: resize! */
const int INITIAL_CAP = 1024;

MidiBuffer *midibuff_new_empty(void) {
    MidiBuffer *new = malloc(sizeof *new);
    if(!new)
        return NULL;
    void *bytemem = malloc(INITIAL_CAP);
    if(!bytemem)
        goto fail;
    new->bytes = bytemem;
    new->used = 0;
    new->cap = INITIAL_CAP;
    memset(new->bytes, 0, INITIAL_CAP);
    return new;

fail:
    free(new);
    return NULL;
}

MidiBuffer *midibuff_new(uint16_t format, uint16_t tracks, uint16_t division) {
    MidiBuffer *new = midibuff_new_empty();
    midibuff_append_header(new, format, tracks, division);
    return new;
}

void midibuff_free(MidiBuffer *this) {
    free(this->bytes);
    free(this);
}

void midibuff_append_raw(MidiBuffer *this, void *data, size_t count) {
    assert(this->used < this->cap);
    memcpy(&NEXT_BYTE(this), data, count);
    this->used += count;
}

void midibuff_append_string(MidiBuffer *this, char *data) {
    size_t count = strlen(data);
    midibuff_append_raw(this, data, count);
}

void midibuff_append_uint32(MidiBuffer *this, uint32_t value) {
    uint32_t data = htonl(value);
    midibuff_append_raw(this, &data, 4);
}

void midibuff_append_uint16(MidiBuffer *this, uint16_t value) {
    uint16_t data = htons(value);
    midibuff_append_raw(this, &data, 2);
}

void midibuff_append_uint8(MidiBuffer *this, uint8_t value) {
    midibuff_append_raw(this, &value, 1);
}

void midibuff_append_header(MidiBuffer *this, uint16_t format, uint16_t tracks, uint16_t division) {
    midibuff_append_string(this, "MThd");
    midibuff_append_uint32(this, 6);        /* num bytes following in header */
    midibuff_append_uint16(this, format);   /* 0=single-track; 1=multi-track; 2=multi-song */
    midibuff_append_uint16(this, tracks);   /* number of tracks following header */
    midibuff_append_uint16(this, division); /* +=ticks per beat; -=SMPTE units */
}

size_t midibuff_save_as(MidiBuffer *this, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if(!f)
        return 0;
    size_t result = fwrite(this->bytes, this->used, 1, f);
    fclose(f);
    return result;
}
