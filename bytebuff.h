#ifndef BYTEBUFF_H
#define BYTEBUFF_H

#include <stdint.h>

struct bytebuff {
    uint8_t *bytes;
    uint8_t *p;
    int cap;
};

struct bytebuff *bytebuff_new(void);
void bytebuff_free(struct bytebuff *this);
void bytebuff_append_raw(struct bytebuff *this, void *data, size_t count);
void bytebuff_append_string(struct bytebuff *this, char *data);
void bytebuff_append_uint32(struct bytebuff *this, uint32_t data);

#endif
