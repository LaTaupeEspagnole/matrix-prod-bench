#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "libmatrix.h"
#include "buffer.h"

char* get_next_line(char* str)
{
  while (*str != '\0' && *str != '\n')
    ++str;
  if (*str == '\0')
    return NULL;
  return str + 1;
}

char* serialize_matrix(struct mat* matrix)
{
  struct buffer buffer = {.size = 0, .buffer = NULL};
  char* tmp = malloc(100 * sizeof(char));
  if (!tmp)
    return NULL;

  sprintf(tmp, "%zu\n", matrix->width);
  add_buffer(&buffer, tmp, strlen(tmp));
  sprintf(tmp, "%zu\n", matrix->height);
  add_buffer(&buffer, tmp, strlen(tmp));
  size_t max = matrix->width * matrix->height;
  for (size_t i = 0; i < max; ++i)
  {
    sprintf(tmp, "%f\n", matrix->array[i]);
    add_buffer(&buffer, tmp, strlen(tmp));
  }
  free(tmp);

  return buffer.buffer;
}

struct mat* deserialize_matrix(char* str, char** new_pos)
{
  unsigned width = 0;
  unsigned height = 0;

  if (!sscanf(str, "%u\n", &width))
    return NULL;
  str = get_next_line(str);
  if (!sscanf(str, "%u\n", &height))
    return NULL;
  str = get_next_line(str);
  struct mat* matrix = dec_mat(width, height);
  unsigned max = width * height;
  for (unsigned i = 0; i < max; ++i)
  {
    if (!sscanf(str, "%f\n", matrix->array + i))
    {
      free_mat(matrix);
      return NULL;
    }
    str = get_next_line(str);
  }
  *new_pos = str;

  for (unsigned lin = 0; lin < matrix->height; ++lin)
  {
    for (unsigned col = 0; col < matrix->width; ++col)
      matrix->array_mirrored[lin + col * matrix->height] = matrix->array[col + lin * matrix->width];
  }

  return matrix;
}

char *serialize_mat(struct mat *a)
{
  if (!a)
    return NULL;

  struct buffer res = {.size = 0, .buffer = NULL};
  char *val = malloc(256 * sizeof (char));
  sprintf(val, "M%zu:%zu:", a->width, a->height);
  add_buffer(&res, val, strlen(val));

  size_t mat_size = a->width * a->height;
  for (size_t i = 0; i < mat_size; ++i)
  {
    sprintf(val, "%f:", a->array[i]);
    add_buffer(&res, val, strlen(val));
  }
  return res.buffer;
}

/*
** Gives the padding in byte to the item according
** to it's index (starting from index 0).
*/
size_t get_padding(char *str, size_t item_index)
{
  char *save = str;
  while (item_index)
  {
    while(*str != ':')
      ++str;
    ++str;
    --item_index;
  }
  return str - save;
}

struct mat *deserialize_mat(char *str)
{
  if (!str)
    return NULL;

  size_t width = 0;
  size_t height = 0;
  int nb_ele = sscanf(str, "M%zu:%zu:", &width, &height);
  if (nb_ele != 2)
  {
    fprintf(stderr, "Invalide format: deseralize_mat()\n");
    return NULL;
  }

  struct mat *res = init_mat(width, height);
  if (!res)
    return NULL;
  size_t mat_size = res->width * res->height;
  size_t padding = get_padding(str, 2);
  for (size_t i = 0; i < mat_size; ++i)
  {
    sscanf(str + padding, "%f:", res->array + i);
    padding += get_padding(str + padding, 1);
  }

  for (unsigned lin = 0; lin < res->height; ++lin)
  {
    for (unsigned col = 0; col < res->width; ++col)
      res->array_mirrored[lin + col * res->height] = res->array[col + lin * res->width];
  }

  return res;
}
