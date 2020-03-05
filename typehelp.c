#include <stdint.h>
#include "typehelp.h"

static const uint32_t ENDIAN_CHECK_VAL = 1;

#define IS_LITTLE_ENDIAN   (((uint8_t *)&ENDIAN_CHECK_VAL)[0])


uint8_t *typehelp_net_order_uint32(uint32_t val, uint8_t *result) {
    if(!IS_LITTLE_ENDIAN) {
        *(uint32_t *)result = val;
        return result;
    }

    uint8_t *bytes = (uint8_t *)&val;
    result[0] = bytes[3];
    result[1] = bytes[2];
    result[2] = bytes[1];
    result[3] = bytes[0];
    return result;
}

uint8_t *typehelp_net_order_uint16(uint16_t val, uint8_t *result) {
    if(!IS_LITTLE_ENDIAN) {
        *(uint32_t *)result = val;
        return result;
    }

    uint8_t *bytes = (uint8_t *)&val;
    result[0] = bytes[1];
    result[1] = bytes[0];
    return result;
}

uint8_t *typehelp_net_order_uint24(uint32_t val, uint8_t *result) {
    if(!IS_LITTLE_ENDIAN) {
        *(uint32_t *)result = val;
        return result;
    }

    uint8_t *bytes = (uint8_t *)&val;
    result[0] = bytes[3];
    result[1] = bytes[2];
    result[2] = bytes[1];
    return result;
}


/* TODO: Deprecate... */


fourbytes flip4(uint32_t in) {
    uint8_t *bytes = (uint8_t *)&in;
    fourbytes result;
    result.bytes[0] = bytes[3];
    result.bytes[1] = bytes[2];
    result.bytes[2] = bytes[1];
    result.bytes[3] = bytes[0];
    return result;
}


threebytes flip3lower(uint32_t in) {
    uint8_t *bytes = (uint8_t *)&in;
    threebytes result;
    result.bytes[0] = bytes[3];
    result.bytes[1] = bytes[2];
    result.bytes[2] = bytes[1];
    return result;
}
