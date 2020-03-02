#ifndef BYTEBUFF_H
#define BYTEBUFF_H

#include <stdint.h>

struct bytebuff {
    uint8_t *bytes;
    size_t used;
    size_t cap;
};

struct bytebuff *bytebuff_new(void);
void bytebuff_free(struct bytebuff *this);
void bytebuff_append_raw(struct bytebuff *this, void *data, size_t count);
void bytebuff_append_string(struct bytebuff *this, char *data);
void bytebuff_append_uint32(struct bytebuff *this, uint32_t data);
void bytebuff_append_uint16(struct bytebuff *this, uint16_t data);
void bytebuff_append_uint8(struct bytebuff *this, uint8_t data);

#endif
