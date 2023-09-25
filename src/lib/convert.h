#ifndef CONVERT_H
#define CONVERT_H

#include "stdint.h"
#include "error.h"

error_t try_uint8_from_str(const char* src, uint8_t* result);

error_t try_uint32_from_str(const char* src, uint32_t* result);

#endif //CONVERT_H