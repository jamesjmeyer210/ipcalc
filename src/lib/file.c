#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include "string.h"
#include "file.h"

inline static off_t file_get_size(FILE* file)
{
  int fd = fileno(file);
  struct stat buf;
  fstat(fd, &buf);
  return buf.st_size;
}

inline static char* file_read_to_str(FILE* file, size_t len, char* dest)
{
  char c;
  for(size_t i = 0; (c = fgetc(file)) != EOF && i < len; i++)
  {
    dest[i] = c;
  }
  return dest;
}

error_t file_read_all_lines(const char* file, Strings* lines)
{
  assert(file != NULL);
  assert(lines == NULL);

  FILE* fptr = fopen(file, "r");
  if(fptr == NULL)
  {
    return ERR_MSG(ERR_FILE_NOT_FOUND, "%s does not exist", file);
  }

  size_t size = file_get_size(fptr);
  char* content = malloc(size);
  if(content == NULL)
  {
    return ERR(ERR_MALLOC_FAIL);
  }
  content = file_read_to_str(fptr, size, content);

  Strings x = str_split(content, '\n');
  memcpy(lines, &x, sizeof(Strings));
  return OK;
}