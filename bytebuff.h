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

#endif
