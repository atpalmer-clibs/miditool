#ifndef TYPEHELP_H
#define TYPEHELP_H

#include <stdint.h>

typedef struct twobytes {
    uint8_t bytes[2];
    uint16_t value;
} twobytes;

typedef struct threebytes {
    uint8_t bytes[3];
} threebytes;

typedef union fourbytes {
    uint8_t bytes[4];
    uint32_t value;
} fourbytes;

twobytes flip2(uint16_t in);
threebytes flip3lower(uint32_t in);
fourbytes flip4(uint32_t in);

#endif
