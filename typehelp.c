#include <stdint.h>
#include "typehelp.h"

static const uint32_t ENDIAN_CHECK_VAL = 1;

#define IS_LITTLE_ENDIAN   (((uint8_t *)&ENDIAN_CHECK_VAL)[0])


uint32_t typehelp_platform_order_uint32(uint8_t *raw) {
    if(!IS_LITTLE_ENDIAN) {
        return *(uint32_t *)raw;
    }

    uint8_t result[4];
    result[0] = raw[3];
    result[1] = raw[2];
    result[2] = raw[1];
    result[3] = raw[0];
    return *(uint32_t *)result;
}


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
