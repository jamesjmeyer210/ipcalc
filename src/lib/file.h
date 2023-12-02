//
// Created by james on 9/27/23.
//

#ifndef IPCALC_FILE_H
#define IPCALC_FILE_H

#include "error.h"
#include "list.h"

error_t file_read_all_lines(const char* file, Strings* lines);

#endif //IPCALC_FILE_H