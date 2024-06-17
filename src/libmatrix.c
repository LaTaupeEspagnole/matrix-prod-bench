#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "libmatrix.h"

struct mat *dec_mat(size_t width, size_t height)
{
  if (!width || !height)
    return NULL;

  struct mat *res = malloc(sizeof (struct mat));
  if (!res)
    return NULL;

  res->width = width;
  res->height = height;
  res->array = aligned_alloc(32, width * height * sizeof (float));
  if (!res->array)
  {
    free(res);
    return NULL;
  }
  res->array_mirrored = aligned_alloc(32, width * height * sizeof (float));
  if (!res->array_mirrored)
  {
    free(res->array);
    free(res);
    return NULL;
  }
  return res;
}

struct mat *init_mat(size_t width, size_t height)
{
  struct mat *res = dec_mat(width, height);
  if (!res)
    return NULL;

  size_t size = width * height;
  for (size_t i = 0; i < size; ++i)
  {
    res->array[i] = 0.0f;
    res->array_mirrored[i] = 0.0f;
  }
  return res;
}

void free_mat(struct mat *m)
{
  if (!m)
    return;

  free(m->array);
  free(m->array_mirrored);
  free(m);
}

struct mat *copy_mat(struct mat *m)
{
  if (!m)
    return NULL;

  struct mat *res = init_mat(m->width, m->height);
  if (!res)
    return NULL;

  size_t prod = m->width * m->height;
  for (size_t i = 0; i < prod; ++i)
  {
    res->array[i] = m->array[i];
    res->array_mirrored[i] = m->array_mirrored[i];
  }

  return res;
}

int is_equal(struct mat *a, struct mat *b)
{
  if (!a || !b || (a->width != b->width) || (a->height != b->height))
    return 0;

  size_t size = a->width * a->height;
  size_t i = 0;
  while (i < size && a->array[i] == b->array[i])
    ++i;
  return i == size;
}

int is_equal_threshold(struct mat *a, struct mat *b, float threshold)
{
  if (!a || !b || (a->width != b->width) || (a->height != b->height))
    return 0;

  size_t size = a->width * a->height;
  size_t i = 0;
  while (i < size && (fabsf(a->array[i] - b->array[i]) <= threshold))
    ++i;
  return i == size;
}

void print_mat(struct mat *a)
{
  if (!a)
  {
    printf("NULL matrix !\n");
    return;
  }
  printf("Width: %zu, Height: %zu\n", a->width, a->height);
  for(size_t l = 0; l < a->height; ++l)
  {
    for (size_t c = 0; c < a->width; ++c)
    {
      printf("%8.2f ", a->array[c + l * a->width]);
    }
    printf("\n");
  }
  printf("\n");
}

static inline unsigned is_same_size(struct mat *a, struct mat *b)
{
  return (a->width == b->width) && (a->height == b->height);
}

static struct mat *add_sub_mat(struct mat *a, struct mat *b, int mult)
{
  if (!a || !b || !is_same_size(a, b))
    return NULL;

  struct mat *res = init_mat(a->width, a->height);
  if (!res)
    return NULL;

  size_t prod = a->width * a->height;
  for (size_t i = 0; i < prod; ++i)
  {
    res->array[i] = a->array[i] + (mult * b->array[i]);
    res->array_mirrored[i] = a->array_mirrored[i] + (mult * b->array_mirrored[i]);
  }

  return res;
}

struct mat *add_mat(struct mat *a, struct mat *b)
{
  return add_sub_mat(a, b, 1);
}

struct mat *sub_mat(struct mat *a, struct mat *b)
{
  return add_sub_mat(a, b, -1);
}

static void add_sub_mat_ip(struct mat *a, struct mat *b, int mult)
{
  if (!a || !b || !is_same_size(a, b))
    return;

  size_t prod = a->width * a->height;
  for (size_t i = 0; i < prod; ++i)
  {
    a->array[i] = a->array[i] + (mult * b->array[i]);
    a->array_mirrored[i] = a->array_mirrored[i] + (mult * b->array_mirrored[i]);
  }
}

void add_mat_ip(struct mat *a, struct mat *b)
{
  add_sub_mat_ip(a, b, 1);
}

void sub_mat_ip(struct mat *a, struct mat *b)
{
  add_sub_mat_ip(a, b, -1);
}

struct mat *div_mat(struct mat *a, float d)
{
  if (!a)
    return NULL;

  struct mat *res = init_mat(a->width, a->height);
  if (!res)
    return NULL;

  size_t prod = a->width * a->height;
  for (size_t i = 0; i < prod; ++i)
  {
    res->array[i] = a->array[i] / d;
    res->array_mirrored[i] = a->array_mirrored[i] / d;
  }

  return res;
}

struct mat *sc_mult_mat(struct mat *a, float m)
{
  if (!a)
    return NULL;

  struct mat *res = init_mat(a->width, a->height);
  if (!res)
    return NULL;

  size_t prod = a->width * a->height;
  for (size_t i = 0; i < prod; ++i)
  {
    res->array[i] = a->array[i] * m;
    res->array_mirrored[i] = a->array_mirrored[i] * m;
  }

  return res;
}

void div_mat_ip(struct mat *a, float d)
{
  if (!a)
    return;

  size_t prod = a->width * a->height;
  for (size_t i = 0; i < prod; ++i)
  {
    a->array[i] = a->array[i] / d;
    a->array_mirrored[i] = a->array_mirrored[i] / d;
  }
}

void sc_mult_mat_ip(struct mat *a, float m)
{
  if (!a)
    return;

  size_t prod = a->width * a->height;
  for (size_t i = 0; i < prod; ++i)
  {
    a->array[i] = a->array[i] * m;
    a->array_mirrored[i] = a->array_mirrored[i] * m;
  }
}

struct mat *trans_mat(struct mat *a)
{
  if (!a)
    return NULL;

  struct mat *res = init_mat(a->height, a->width);
  if (!res)
    return NULL;

  size_t prod = a->width * a->height;
  for (size_t i = 0; i < prod; ++i)
  {
    res->array[i] = a->array_mirrored[i];
    res->array_mirrored[i] = a->array[i];
  }

  return res;
}
