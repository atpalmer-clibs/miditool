#ifndef TYPEHELP_H
#define TYPEHELP_H

#include <stdint.h>

uint8_t *typehelp_net_order_uint32(uint32_t val, uint8_t *result);
uint8_t *typehelp_net_order_uint16(uint16_t val, uint8_t *result);


/* TODO: Deprecate... */

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
