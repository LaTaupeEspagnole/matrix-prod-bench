#ifndef MULTMAT_H
# define MULTMAT_H

# include <stddef.h>

# include "libmatrix.h"

struct inter
{
  size_t begin;
  size_t end;
};

struct thread_info
{
  const struct mat *a;
  const struct mat *b;
  struct mat *res;
  struct inter inter;
  float (*comput_case_func)(const struct mat *a,
                           const struct mat *b,
                           const struct mat *res,
                           const size_t pos);
};

#endif /* !MULTMAT_H */
