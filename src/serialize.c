#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "libmatrix.h"
#include "buffer.h"

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

struct mat *deseralize_mat(char *str)
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
  size_t mat_size = res->width * res->height;
  size_t padding = get_padding(str, 2);
  for (size_t i = 0; i < mat_size; ++i)
  {
    sscanf(str + padding, "%f:", res->array + i);
    padding += get_padding(str + padding, 1);
  }

  return res;
}
