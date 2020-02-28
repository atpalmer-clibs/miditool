#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "bytebuff.h"

const int INITIAL_CAP = 1024;

struct bytebuff *bytebuff_new(void) {
    struct bytebuff *new = malloc(sizeof *new);
    new->bytes = malloc(INITIAL_CAP);
    new->p = new->bytes;
    new->cap = INITIAL_CAP;
    memset(new->bytes, 0, INITIAL_CAP);
    return new;
}

void bytebuff_free(struct bytebuff *this) {
    free(this->bytes);
    free(this);
}

void bytebuff_append_raw(struct bytebuff *this, void *data, size_t count) {
    assert(this->p - this->bytes < this->cap);
    memcpy(this->bytes, data, count);
    this->p += count;
}

void bytebuff_append_string(struct bytebuff *this, char *data) {
    size_t count = strlen(data);
    bytebuff_append_raw(this, data, count);
}
