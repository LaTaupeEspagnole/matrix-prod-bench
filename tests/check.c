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

void show_mat(struct mat *a, struct mat *b, struct mat *e, struct mat *o)
{
  printf("The two results are not equal !\n");
  printf("(mat(%zu, %zu) * mat(%zu, %zu))\n", a->width, a->height, b->width, b->height);
  printf("A:\n");
  print_mat(a);
  printf("B:\n");
  print_mat(b);
  printf("Expected:\n");
  print_mat(e);
  printf("Optained:\n");
  print_mat(o);
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s [max_mat_size]\n", argv[0]);
    return 0;
  }

  srand(time(NULL));
  size_t size_mat = atoi(argv[1]);
  size_t nb_threads = 4;
  size_t errors = 0;
  size_t tests = 0;

  printf("> Verifying all kind of multiplication of max size %zu x %zu matrix\n", size_mat, size_mat);
  printf("> Using %zu threads\n", nb_threads);

  for (size_t h = 1; h < size_mat; ++h)
  {
    for (size_t w = 1; w < size_mat; ++w)
    {
      struct mat *a = init_mat(w, h);
      struct mat *b = init_mat(h, w);
      init_random(a);
      init_random(b);
      struct mat *res1 = mult_mat(a, b);
      struct mat *res2 = mult_mat_th(a, b, nb_threads);
      struct mat *res3 = mult_mat(b, a);
      struct mat *res4 = mult_mat_th(b, a, nb_threads);

      if (!is_equal(res1, res2))
      {
        show_mat(a, b, res1, res2);
        errors += 1;
      }
      if (!is_equal(res3, res4))
      {
        show_mat(a, b, res3, res4);
        errors += 1;
      }
      tests += 2;

      free_mat(a);
      free_mat(b);
      free_mat(res1);
      free_mat(res2);
      free_mat(res3);
      free_mat(res4);
    }
  }
  printf("Finished: (%zu errors / %zu tests)\n", errors, tests);

  return 0;
}
