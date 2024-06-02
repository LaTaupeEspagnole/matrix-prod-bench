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
    printf("Usage: %s [mat_size] [nb_threads]\n", argv[0]);
    return 0;
  }

  clock_t begin = clock();

  srand(time(NULL));
  size_t size_mat = atoi(argv[1]);
  size_t nb_threads = atoi(argv[2]);
  struct mat *a = init_mat(size_mat, size_mat);
  struct mat *b = init_mat(size_mat, size_mat);
  init_random(a);
  init_random(b);

  clock_t end = clock();
  printf("Random generation time : %f\n", (double)(end - begin) / CLOCKS_PER_SEC);

  clock_t begin_process = clock();
//  struct mat *res = mult_mat_th_simd(a, b, nb_threads);
  struct mat *res = mult_mat_th_naive(a, b, nb_threads);
  clock_t end_process = clock();

  printf("Compute time : %f\n", (double)(end_process - begin_process) / CLOCKS_PER_SEC);

  free_mat(a);
  free_mat(b);
  free_mat(res);

  return 0;
}
