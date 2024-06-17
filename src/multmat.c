#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include <immintrin.h>

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
  a->array_mirrored[lin + col * a->height] = v;
}

static inline void add_val_mat(const struct mat *a,
                               const size_t lin,
                               const size_t col,
                               const float v)
{
  a->array[col + lin * a->width] += v;
  a->array_mirrored[lin + col * a->height] += v;
}

static inline size_t is_mult_ok(const struct mat *a, const struct mat *b)
{
  return a->width == b->height;
}

static inline void comput_case_naive(const struct mat *a,
                                     const struct mat *b,
                                     const struct mat *res,
                                     const size_t lin,
                                     const size_t col)
{
  for (size_t i = 0; i < a->width; i++)
    add_val_mat(res, lin, col, get_val_mat(a, lin, i) * get_val_mat(b, i, col));
}

static inline void comput_case_simd(const struct mat *a,
                                    const struct mat *b,
                                    const struct mat *res,
                                    const size_t lin,
                                    const size_t col)
{
  const size_t widthA = a->width;
  const size_t limit = (a->width / 8) * 8;
  const size_t lin_X_widthA = lin * widthA;
  const size_t col_X_heightB = col * b->height;

  __attribute__ ((aligned (32))) float matA[8], matB[8], resVals[8];
  __m256 resSum = _mm256_set_ps(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

  for (size_t i = 0; i < limit; i += 8) {

    memcpy(matA, a->array + i + lin_X_widthA, 4 * 8);
    memcpy(matB, b->array_mirrored + i + col_X_heightB, 4 * 8);

    __m256 vect_matA = _mm256_load_ps(matA);
    __m256 vect_matB = _mm256_load_ps(matB);

    resSum = _mm256_fmadd_ps(vect_matA, vect_matB, resSum);
  }

  __m256 tmp = _mm256_hadd_ps(resSum, resSum);
  _mm256_store_ps(resVals, tmp);
  float tmpRes = resVals[0] + resVals[1] + resVals[4] + resVals[5];

  for (size_t i = limit; i < a->width; i++)
    tmpRes += get_val_mat(a, lin, i) * get_val_mat(b, i, col);

  set_val_mat(res, lin, col, tmpRes);
}

static void mult_mat_inter(const struct mat *a,
                           const struct mat *b,
                           const struct mat *res,
                           const struct inter inter,
                           void (*comput_case_func)(const struct mat *a,
                                                    const struct mat *b,
                                                    const struct mat *res,
                                                    const size_t lin,
                                                    const size_t col))
{
  const size_t start_lin = inter.begin / res->width;
  const size_t start_col = inter.begin % res->width;
  const size_t end_lin = inter.end / res->width;
  const size_t end_col = inter.end % res->width;

  if (start_lin == end_lin)
  {
    for (size_t c = start_col; c < end_col; ++c)
      (*comput_case_func)(a, b, res, start_lin, c);
    return;
  }

  for (size_t c = start_col; c < res->width; ++c)
    (*comput_case_func)(a, b, res, start_lin, c);

  if (end_lin != start_lin + 1)
  {
    for (size_t l = start_lin + 1; l < end_lin; ++l)
    {
      for (size_t c = 0; c < res->width; ++c)
        (*comput_case_func)(a, b, res, l, c);
    }
  }

  for (size_t c = 0; c < end_col; ++c)
    (*comput_case_func)(a, b, res, end_lin, c);
}

static inline struct mat *mult_mat(const struct mat *a,
                     const struct mat *b,
                     void (*comput_case_func)(const struct mat *a,
                                              const struct mat *b,
                                              const struct mat *res,
                                              const size_t lin,
                                              const size_t col))
{
  if (!a || !b || !is_mult_ok(a, b))
    return NULL;

  struct mat *r = init_mat(b->width, a->height);
  mult_mat_inter(a, b, r, init_inter(0, r->width * r->height), comput_case_func);
  return r;
}

struct mat *mult_mat_naive(const struct mat *a, const struct mat *b)
{
  return mult_mat(a, b, comput_case_naive);
}

struct mat *mult_mat_simd(const struct mat *a, const struct mat *b)
{
  return mult_mat(a, b, comput_case_simd);
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
  ti.comput_case_func = comput_case_naive;
  return ti;
}

static void *warper_mult_mat_inter(void *arg)
{
  struct thread_info *ti = arg;
  mult_mat_inter(ti->a, ti->b, ti->res, ti->inter, ti->comput_case_func);
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

struct mat *mult_mat_th(const struct mat *a,
                        const struct mat *b,
                        const size_t nb_threads,
                        void (*comput_case_func)(const struct mat *a,
                                                 const struct mat *b,
                                                 const struct mat *res,
                                                 const size_t lin,
                                                 const size_t col))
{
  if (!a || !b || !is_mult_ok(a, b))
    return NULL;

  float size_res = b->width * a->height;
  if (size_res < nb_threads)
    return mult_mat(a, b, comput_case_func);

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
    ti[thread].comput_case_func = comput_case_func;
    ++thread;
  }

  start_threads(ti, nb_threads);
  free(ti);
  return r;
}

struct mat *mult_mat_th_naive(const struct mat *a,
                              const struct mat *b,
                              const size_t nb_threads)
{
  return mult_mat_th(a, b, nb_threads, comput_case_naive);
}

struct mat *mult_mat_th_simd(const struct mat *a,
                             const struct mat *b,
                             const size_t nb_threads)
{
  return mult_mat_th(a, b, nb_threads, comput_case_simd);
}

struct mat *mult_mat_auto(struct mat *a, struct mat *b,
                          size_t nb_threads,
                          size_t threshold)
{
  if (!a || !b || !is_mult_ok(a, b))
    return NULL;

  size_t a_size = a->width * a->height;
  size_t b_size = b->width * b->height;
  if (a_size < b_size)
    a_size = b_size;
  if (a_size < threshold)
    return mult_mat_naive(a, b);
  return mult_mat_th_naive(a, b, nb_threads);
}

