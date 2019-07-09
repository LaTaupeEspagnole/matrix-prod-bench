#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "libmatrix.h"

void init_random(struct mat *a)
{
  size_t size = a->width * a->height;
  for (size_t i = 0; i < size; ++i)
  {
    a->array[i] = (float)(rand() % 1000) / 100;
  }
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("Usage: %s [TYPE] [mat_size]\n", argv[0]);
    printf("  TYPES:\n");
    printf("    mul\n");
    printf("    mul_simd\n");
    printf("    mul_th\n");
    printf("    mul_th_simd\n");
    return 0;
  }

  srand(time(NULL));
  size_t size_mat = atoi(argv[2]);

  printf("Initiation of the matrices...\n");
  struct mat *a = init_mat(size_mat, size_mat);
  struct mat *b = init_mat(size_mat, size_mat);
  struct mat *res = NULL;

  printf("Setting random values in the matrices...\n");
  init_random(a);
  init_random(b);

  if (strcmp(argv[1], "mul_th_simd") == 0) {
    printf("Processing threaded SIMD...\n");
    res = mult_mat_th_simd(a, b, 4);
  }
  else if (strcmp(argv[1], "mul_th") == 0) {
    printf("Processing threaded naive...\n");
    res = mult_mat_th_naive(a, b, 4);
  }
  else if (strcmp(argv[1], "mul_simd") == 0) {
    printf("Processing SIMD...\n");
    res = mult_mat_simd(a, b);
  }
  else if (strcmp(argv[1], "mul") == 0) {
    printf("Processing naive...\n");
    res = mult_mat_naive(a, b);
  }
/*
  printf("Comparing patrices...\n");
  if (!is_equal_threshold(res_simd, res_naive, 0.1f))
    printf("> Matrices not equal.\n");
  else
    printf("> Matrices are equal.\n");
*/
/*
  print_mat(res_naive);
  printf("\n");
  print_mat(res_simd);
*/
  printf("Freeing ressources...\n");
  free_mat(a);
  free_mat(b);
  if (res)
    free_mat(res);

  return 0;
}
