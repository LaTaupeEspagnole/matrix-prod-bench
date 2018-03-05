#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#include "multmat.h"

struct inter init_inter(const size_t begin, const size_t end)
{
  struct inter inter = {.begin = begin, .end = end};
  return inter;
}

static inline float get_val_mat(const struct mat *a,
                                const size_t lin,
                                const size_t col)
{
  return a->array[col + lin * a->width];
}

static inline void set_val_mat(const struct mat *a,
                               const size_t lin,
                               const size_t col,
                               const float v)
{
  a->array[col + lin * a->width] = v;
}

static inline void add_val_mat(const struct mat *a,
                               const size_t lin,
                               const size_t col,
                               const float v)
{
  a->array[col + lin * a->width] += v;
}

static inline size_t is_mult_ok(const struct mat *a, const struct mat *b)
{
  return a->width == b->height;
}

static inline void comput_case(const struct mat *a,
                               const struct mat *b,
                               const struct mat *res,
                               const size_t lin,
                               const size_t col)
{
  set_val_mat(res, lin, col, get_val_mat(a, lin, 0) * get_val_mat(b, 0, col));
  for (size_t i = 1; i < a->width; ++i)
    add_val_mat(res, lin, col, get_val_mat(a, lin, i) * get_val_mat(b, i, col));
}

static void mult_mat_inter(const struct mat *a,
                           const struct mat *b,
                           const struct mat *res,
                           const struct inter inter)
{
  size_t start_lin = inter.begin / res->width;
  size_t start_col = inter.begin % res->width;
  size_t end_lin = inter.end / res->width;
  size_t end_col = inter.end % res->width;

  if (start_lin == end_lin)
  {
    for (size_t c = start_col; c < end_col; ++c)
      comput_case(a, b, res, start_lin, c);
    return;
  }

  for (size_t c = start_col; c < res->width; ++c)
    comput_case(a, b, res, start_lin, c);

  if (end_lin != start_lin + 1)
  {
    for (size_t l = start_lin + 1; l < end_lin; ++l)
    {
      for (size_t c = 0; c < res->width; ++c)
        comput_case(a, b, res, l, c);
    }
  }

  for (size_t c = 0; c < end_col; ++c)
    comput_case(a, b, res, end_lin, c);
}

static inline struct thread_info init_thread_info(const struct mat *a,
                                                  const struct mat *b,
                                                  struct mat *res)
{
  struct thread_info ti;
  ti.a = a;
  ti.b = b;
  ti.res = res;
  ti.inter.begin = 0;
  ti.inter.end = 0;
  return ti;
}

static void *warper_mult_mat_inter(void *arg)
{
  struct thread_info *ti = arg;
  mult_mat_inter(ti->a, ti->b, ti->res, ti->inter);
  return NULL;
}

static void start_threads(struct thread_info *ti, const size_t nb_threads)
{
  pthread_t *tid = malloc(nb_threads * sizeof (pthread_t));
  if (!ti)
    exit(1);

  for (size_t i = 0; i < nb_threads; ++i)
    pthread_create(tid + i, NULL, warper_mult_mat_inter, ti + i);
  for (size_t i = 0; i < nb_threads; ++i)
    pthread_join(tid[i], NULL);
  free(tid);
}

struct mat *mult_mat_th(const struct mat *a, const struct mat *b, const size_t nb_threads)
{
  if (!a || !b || !is_mult_ok(a, b))
    return NULL;

  float size_res = b->width * a->height;
  if (size_res < nb_threads)
    return mult_mat(a, b);

  struct thread_info *ti = malloc(nb_threads * sizeof (struct thread_info));
  size_t thread = 0;
  const float nb_threads_f = nb_threads;
  struct mat *r = init_mat(b->width, a->height);

  for (float i = 0.0f; i < nb_threads_f; i += 1.0f)
  {
    float frac = size_res / nb_threads_f;
    float begi = i * frac;
    ti[thread] = init_thread_info(a, b, r);
    ti[thread].inter.begin = roundf(begi);
    if (i >= nb_threads_f)
      ti[thread].inter.end = size_res;
    else
      ti[thread].inter.end = roundf(begi + frac);
    ++thread;
  }

  start_threads(ti, nb_threads);
  free(ti);
  return r;
}

struct mat *mult_mat(const struct mat *a, const struct mat *b)
{
  if (!a || !b || !is_mult_ok(a, b))
    return NULL;

  struct mat *r = init_mat(b->width, a->height);
  mult_mat_inter(a, b, r, init_inter(0, r->width * r->height));
  return r;
}
