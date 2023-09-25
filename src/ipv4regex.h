#ifndef IPV4REGEX_H
#define IPV4REGEX_H

#include <regex.h>
#include <stdbool.h>

regex_t init_ipv4_regex();

regex_t init_ipv4_range_regex();

bool regex_is_valid(const regex_t* regex, const char* str);

#endif //IPV4REGEX_H