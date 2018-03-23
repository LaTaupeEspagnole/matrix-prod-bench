#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libmatrix.h"

void init_random(struct mat *a)
{
  size_t size = a->width * a->height;
  for (size_t i = 0; i < size; ++i)
  {
    a->array[i] = (float)(rand() % 10000) / 100;
  }
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage: %s [mat_width] [mat_height]\n", argv[0]);
    return 0;
  }

  srand(time(NULL));
  size_t mat_width = atoi(argv[1]);
  size_t mat_height = atoi(argv[2]);
  struct mat *a = init_mat(mat_width, mat_height);
  init_random(a);

  print_mat(a);
  printf("----------------\n\n");
  char *res = serialize_mat(a);
  printf("%s\n\n", res);
  printf("----------------\n\n");
  struct mat *b = deseralize_mat(res);
  print_mat(b);

  if (is_equal(a, b))
    printf("The two matrix are equal.\n");

  free_mat(a);
  free_mat(b);
  free(res);

  return 0;
}
