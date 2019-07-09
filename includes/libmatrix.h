#ifndef LIBMATRIX_H
# define LIBMATRIX_H

# include <stddef.h>

struct mat
{
  size_t width;
  size_t height;
  float *array;
};

/*
** All the functions that return a pointer can return NULL
** if an error occured.
*/

/*
** Inits a struct mat with not initialised values
** (more performant that init_mat()).
*/
struct mat *dec_mat(size_t width, size_t height);

/* Inits a struct mat initialized with zero's. */
struct mat *init_mat(size_t width, size_t height);

/* Frees a struct mat and the array in it. */
void free_mat(struct mat *m);

/*
** Copys a struct mat in another newly allocated one
** and returns it's pointer. The array is also copyed.
*/
struct mat *copy_mat(struct mat *m);

/* Checks if matrix a and b are equal */
int is_equal(struct mat *a, struct mat *b);

/* Checks if matrix a and b are equal withing a threshold */
int is_equal_threshold(struct mat *a, struct mat *b, float threshold);

/* Prints mat a */
void print_mat(struct mat *a);

/*
** Adds matrix a and b, stors the result in a new
** matrix and returns it's pointer.
*/
struct mat *add_mat(struct mat *a, struct mat *b);

/* Adds matrix a and b, stors the result in a. */
void add_mat_ip(struct mat *a, struct mat *b);

/*
** Substracts matrix a and b, stors the result in a new
** matrix and returns it's pointer.
*/
struct mat *sub_mat(struct mat *a, struct mat *b);

/* Substracts matrix a and b, stors the result in a. */
void sub_mat_ip(struct mat *a, struct mat *b);

/*
** Multiplies matrix a and b, stors the result in a new
** matrix and returns it's pointer using the naive algorithm.
*/
struct mat *mult_mat_naive(const struct mat *a, const struct mat *b);

/*
** Multiplies matrix a and b, stors the result in a new
** matrix and returns it's pointer using SIMD intructions.
*/
struct mat *mult_mat_simd(const struct mat *a, const struct mat *b);

/*
** Same as mult_mat_naive() but using nb_threads threads.
*/
struct mat *mult_mat_th_naive(const struct mat *a,
                              const struct mat *b,
                              const size_t nb_threads);

/*
** Same as mult_mat_simd() but using nb_threads threads.
*/
struct mat *mult_mat_th_simd(const struct mat *a,
                             const struct mat *b,
                             const size_t nb_threads);

/*
** Same as mult_mat() if
** max(a->width x a->heigth, b->width x b->height) < threshold
** Otherwise same as mult_mat_th() with nb_threads threads.
*/
struct mat *mult_mat_auto(struct mat *a, struct mat *b,
                          size_t nb_threads,
                          size_t threshold);

/*
** Divides matrix a by d and stors the result in a new
** matrix and returns it
*/
struct mat *div_mat(struct mat *a, float d);

/* Divides matrix a by d and stors the result in a */
void div_mat_ip(struct mat *a, float d);

/*
** Multiplies matrix a by d and stors the result in a new
** matrix and returns it
*/
struct mat *sc_mult_mat(struct mat *a, float m);

/* Multiplies matrix a by d and stors the result in a */
void sc_mult_mat_ip(struct mat *a, float m);

/* Returns the transpose matrix of a */
struct mat *trans_mat(struct mat *a);

/* Serialize a matrix into a string */
char* serialize_matrix(struct mat* matrix);

/* Deserialize a string into a matrix */
struct mat* deserialize_matrix(char* str, char** new_pos);

/* Serialize a matrix into a string (deprecate) */
char *serialize_mat(struct mat *a);

/* Deserialize a string into a matrix (deprecate) */
struct mat *deserialize_mat(char *str);

#endif /* !LIBMATRIX_H */
