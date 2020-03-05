#ifndef TYPEHELP_H
#define TYPEHELP_H

#include <stdint.h>

uint32_t typehelp_platform_order_uint32(uint8_t *raw);

uint8_t *typehelp_net_order_uint32(uint32_t val, uint8_t *result);
uint8_t *typehelp_net_order_uint16(uint16_t val, uint8_t *result);

/* accepts 32-bit type and writes bottom three bytes into result */
uint8_t *typehelp_net_order_uint24(uint32_t val, uint8_t *result);

#endif
