#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "buffer.h"

struct buffer *init_buffer(void)
{
  struct buffer *buffer = malloc(sizeof(struct buffer));
  if (!buffer)
  {
    perror("cannot do malloc()");
    exit(3);
  }
  buffer->size = 0;
  buffer->buffer = NULL;
  return buffer;
}

void add_buffer(struct buffer *buffer, const char *str, size_t size)
{
  if (!str || !size || !buffer)
    return;

  char *save = realloc(buffer->buffer, buffer->size + size + 1);
  if(!save)
  {
    perror("cannot do realloc()");
    exit(3);
  }
  if (save != buffer->buffer)
    buffer->buffer = save;
  memcpy(buffer->buffer + buffer->size, str, size);
  buffer->size += size;
  buffer->buffer[buffer->size] = '\0';
}

void reduce_buffer(struct buffer *buffer, size_t new_size)
{
  if (!buffer || new_size > buffer->size)
    return;

  char *save = realloc(buffer->buffer, new_size + 1);
  if (!save)
  {
    perror("cannot do realloc()");
    exit(3);
  }
  if (save != buffer->buffer)
    buffer->buffer = save;
  buffer->size = new_size;
  buffer->buffer[buffer->size] = '\0';
}

void empty_buffer(struct buffer *buffer)
{
  if (!buffer)
    return;

  free(buffer->buffer);
  buffer->buffer = NULL;
  buffer->size = 0;
}

void free_buffer(struct buffer *buffer)
{
  if (!buffer)
    return;

  if (!buffer->buffer)
    free(buffer);
  else
  {
    free(buffer->buffer);
    free(buffer);
  }
}
