#ifndef TYPEHELP_H
#define TYPEHELP_H

#include <stdint.h>

typedef struct twobytes {
    char bytes[2];
} twobytes;

typedef union fourbytes {
    char bytes[4];
    uint32_t value;
} fourbytes;

twobytes flip2(uint16_t in);
fourbytes flip4(uint32_t in);

#endif
