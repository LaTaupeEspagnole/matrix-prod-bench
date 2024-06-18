#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "libmatrix.h"

void init_random(struct mat *a)
{
  size_t size = a->width * a->height;
  for (size_t i = 0; i < size; ++i)
  {
    float tmp = (float)(rand() % 10000) / 100;
    a->array[i] = tmp;
    a->array_mirrored[(i / a->width) + (i % a->width) * a->height] = tmp;
  }
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage: %s [mat_size] [nb_threads]\n", argv[0]);
    return 0;
  }

  struct timespec start, finish;
  double elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

  srand(time(NULL));
  size_t size_mat = atoi(argv[1]);
  size_t nb_threads = atoi(argv[2]);
  struct mat *a = init_mat(size_mat, size_mat);
  struct mat *b = init_mat(size_mat, size_mat);
  init_random(a);
  init_random(b);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
  printf("Random generation time : %f\n", (double)elapsed);

  clock_gettime(CLOCK_MONOTONIC, &start);
  struct mat *res = mult_mat_th_simd(a, b, nb_threads);
  //struct mat *res = mult_mat_th_naive(a, b, nb_threads);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  elapsed = (finish.tv_sec - start.tv_sec);
  elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
  printf("Compute time : %f\n", (double)elapsed);

  free_mat(a);
  free_mat(b);
  free_mat(res);

  return 0;
}
