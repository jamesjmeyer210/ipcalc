//
// Created by james on 9/25/23.
//

#ifndef IPCALC_ERROR_H
#define IPCALC_ERROR_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define error_t uint8_t

#define OK 0
#define ERR_ARG_NULL 1
#define ERR_ARG_INVALID 3
#define ERR_ARG_OUT_OF_RANGE 4
#define ERR_FILE_NOT_FOUND 5
#define ERR_MALLOC_FAIL 6

#define ERR_MSG_SIZE 256

typedef struct error {
  error_t code;
  uint16_t line;
  char file[64];
  char func[64];
  char msg[ERR_MSG_SIZE];
} Error;

error_t error_new(error_t error, uint16_t line, const char* file, const char* func);

Error error_new_get(error_t error, uint16_t line, const char* file, const char* func);

error_t error_new_msg(error_t error, uint16_t line, const char* file, const char* func, const char* format, ...)
__THROWNL __attribute__ ((__format__ (__printf__, 5, 6)));

Error error_new_msg_get(error_t error, uint16_t line, const char* file, const char* func, const char* format, ...)
__THROWNL __attribute__ ((__format__ (__printf__, 5, 6)));

Error get_error();

error_t get_error_code();

void print_error(const Error* error, bool verbose);

#define ERR(ERROR_T) error_new(ERROR_T, __LINE__, __FILE_NAME__, __FUNCTION__)

#define ERR_GET(ERROR_T) error_new_get(ERROR_T, __LINE__, __FILE_NAME__, __FUNCTION__)

#define ERR_MSG(ERROR_T, FORMAT, ...) error_new_msg(ERROR_T, __LINE__, __FILE_NAME__, __FUNCTION__, FORMAT, __VA_ARGS__)

#define ERR_MSG_GET(ERROR_T, FORMAT, ...) error_new_msg_get(ERROR_T, __LINE__, __FILE_NAME__, __FUNCTION__, FORMAT, __VA_ARGS__)

#define ERR_IF_NULL(arg, arg_name) if(NULL == arg) return ERR_MSG(ERR_ARG_NULL, "Argument %s must not be NULL", arg_name)

#endif //IPCALC_ERROR_H