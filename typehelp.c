#include <stdint.h>
#include "typehelp.h"


twobytes flip2(uint16_t in) {
    char *bytes = (char *)&in;
    twobytes result;
    result.bytes[0] = bytes[1];
    result.bytes[1] = bytes[0];
    return result;
}


fourbytes flip4(uint32_t in) {
    char *bytes = (char *)&in;
    fourbytes result;
    result.bytes[0] = bytes[3];
    result.bytes[1] = bytes[2];
    result.bytes[2] = bytes[1];
    result.bytes[3] = bytes[0];
    return result;
}
