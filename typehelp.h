#ifndef TYPEHELP_H
#define TYPEHELP_H

#include <stdint.h>

uint8_t *typehelp_net_order_uint32(uint32_t val, uint8_t *result);
uint8_t *typehelp_net_order_uint16(uint16_t val, uint8_t *result);

/* accepts 32-bit type and writes bottom three bytes into result */
uint8_t *typehelp_net_order_uint24(uint32_t val, uint8_t *result);


/* TODO: Deprecate... */

typedef union fourbytes {
    uint8_t bytes[4];
    uint32_t value;
} fourbytes;

fourbytes flip4(uint32_t in);

#endif
