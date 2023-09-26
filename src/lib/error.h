//
// Created by james on 9/25/23.
//

#ifndef IPCALC_ERROR_H
#define IPCALC_ERROR_H

#include <stdint.h>
#include <string.h>

#define error_t uint8_t

#define OK 0
#define ERR_ARG_NULL 1
#define ERR_ARG_INVALID 3
#define ERR_ARG_OUT_OF_RANGE 4

typedef struct error {
  error_t code;
  uint16_t line;
  char file[64];
  char func[64];
  char msg[256];
} Error;

error_t error_new(error_t error, uint16_t line, const char* file, const char* func);

#define ERR(ERROR_T) error_new(ERROR_T, __LINE__, __FILE_NAME__, __FUNCTION__)

error_t error_new_msg(error_t error, uint32_t line, const char* file, const char* func, const char* msg);

#define ERR_MSG(ERROR_T, MSG) error_new_msg(ERROR_T, __LINE__, __FILE_NAME__, __FUNCTION__, MSG)

Error get_error();

error_t get_error_code();

void print_error(const Error* error);

#endif //IPCALC_ERROR_H