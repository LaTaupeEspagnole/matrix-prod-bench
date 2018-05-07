#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "libmatrix.h"
#include "buffer.h"

/*
** Gets the item at index 'index_item' in the string
** (index are separated by ';')
*/
struct buffer *get_item(char *str, size_t index_item)
{
  while (index_item != 0)
  {
    if (*str == '\0')
      return NULL;
    while (*str != ';')
    {
      ++str;
      if (*str == '\0')
        return NULL;
    }
    ++str;
    --index_item;
  }

  struct buffer *res = init_buffer();
  if (!res)
    return NULL;

  while (*str != '\0')
  {
    add_buffer(res, str, 1);
    ++str;
  }
  return res;
}

void get_input(struct buffer *buffer)
{
  char c = EOF;
  c = getchar();
  while (c != EOF && c != '\n')
  {
    add_buffer(buffer, &c, 1);
    c = getchar();
  }
}

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Usage: %s [help|command]\n", argv[0]);
    return 1;
  }

  if (!strcmp(argv[1], "help"))
  {
    printf("Help : ");
    printf("This program takes a command as parameter and stdin\n");
    printf("data and returns stdout data related to the command\n\n");
    printf("List of the commands and the related function called :\n\n");
    printf("  +---------+-----------------+--------------------------+--------------+\n");
    printf("  | Command | Function        | stdin                    | stdout       |\n");
    printf("  +---------+-----------------+--------------------------+--------------+\n");
    printf("  | dec     | dec_mat()       | num1;num2;               | 1 matrix     |\n");
    printf("  | ini     | init_mat()      | num1;num2;               | 1 matrix     |\n");
    printf("  | cpy     | copy_mat()      | mat;                     | 1 matrix     |\n");
    printf("  | ieq     | is_equal()      | mat1;mat2;               | [True|False] |\n");
    printf("  | add     | add_mat()       | mat1;mat2;               | 1 matrix     |\n");
    printf("  | adi     | add_mat_ip()    | mat1;mat2;               | 1 matrix     |\n");
    printf("  | sub     | sub_mat()       | mat1;mat2;               | 1 matrix     |\n");
    printf("  | sui     | sub_mat_ip()    | mat1;mat2;               | 1 matrix     |\n");
    printf("  | mul     | mult_mat()      | mat1;mat2;               | 1 matrix     |\n");
    printf("  | mut     | mult_mat_th()   | mat1;mat2;thrds;         | 1 matrix     |\n");
    printf("  | mua     | mult_mat_auto() | mat1;mat2;thrds;thrshld; | 1 matrix     |\n");
    printf("  | div     | div_mat()       | mat1;float;              | 1 matrix     |\n");
    printf("  | dii     | div_mat_ip()    | mat1;float;              | 1 matrix     |\n");
    printf("  | scm     | sc_mult_mat()   | mat1;float;              | 1 matrix     |\n");
    printf("  | tra     | trans_mat()     | mat;                     | 1 matrix     |\n");
    printf("  +---------+-----------------+--------------------------+--------------+\n");
    printf("\n");
    printf("Return values :\n");
    printf(" 0 -> Everything whent well.\n");
    printf(" 1 -> Not a valid command.\n");
    printf(" 2 -> Memory allocation problems.\n");
    printf(" 3 -> Invalid input (parsing error).\n");
    printf(" 4 -> Invalid command (help to list commands).\n");
    printf(" 5 -> Function error (the function returned NULL).\n");
    printf("\n");
    return 0;
  }

  struct buffer *buffer = init_buffer();
  if (!buffer)
  {
    perror("init_buffer: Unable to create buffer");
    return 2;
  }

  if (!strcmp(argv[1], "dec"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    size_t num1 = atoi(item1->buffer);
    size_t num2 = atoi(item2->buffer);
    struct mat *a = dec_mat(num1, num2);
    if (!a)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(a);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(a);
    free(res);
  }
  else if (!strcmp(argv[1], "ini"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    size_t num1 = atoi(item1->buffer);
    size_t num2 = atoi(item2->buffer);
    struct mat *a = init_mat(num1, num2);
    if (!a)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(a);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(a);
    free(res);
  }
  else if (!strcmp(argv[1], "cpy"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    if (!item1)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in = deserialize_mat(item1->buffer);
    if (!in)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *out = copy_mat(in);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_mat(in);
    free_mat(out);
    free(res);
  }
  else if (!strcmp(argv[1], "ieq"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *in2 = deserialize_mat(item2->buffer);
    if (is_equal(in1, in2))
      printf("True\n");
    else
      printf("False\n");
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(in2);
  }
  else if (!strcmp(argv[1], "add"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *in2 = deserialize_mat(item2->buffer);
    struct mat *out = add_mat(in1, in2);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(in2);
    free_mat(out);
    free(res);
  }
  else if (!strcmp(argv[1], "adi"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *in2 = deserialize_mat(item2->buffer);
    add_mat_ip(in1, in2);
    char *res = serialize_mat(in1);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(in2);
    free(res);
  }
  else if (!strcmp(argv[1], "sub"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *in2 = deserialize_mat(item2->buffer);
    struct mat *out = sub_mat(in1, in2);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(in2);
    free_mat(out);
    free(res);
  }
  else if (!strcmp(argv[1], "sui"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *in2 = deserialize_mat(item2->buffer);
    sub_mat_ip(in1, in2);
    char *res = serialize_mat(in1);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(in2);
    free(res);
  }
  else if (!strcmp(argv[1], "mul"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *in2 = deserialize_mat(item2->buffer);
    struct mat *out = mult_mat(in1, in2);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(in2);
    free_mat(out);
    free(res);
  }
  else if (!strcmp(argv[1], "mut"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    struct buffer *item3 = get_item(buffer->buffer, 2);
    if (!item1 || !item2 || !item3)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *in2 = deserialize_mat(item2->buffer);
    int nb_threads = atoi(item3->buffer);
    struct mat *out = mult_mat_th(in1, in2, nb_threads);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(in2);
    free_mat(out);
    free(res);
  }
  else if (!strcmp(argv[1], "mua"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    struct buffer *item3 = get_item(buffer->buffer, 2);
    struct buffer *item4 = get_item(buffer->buffer, 3);
    if (!item1 || !item2 || !item3)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *in2 = deserialize_mat(item2->buffer);
    unsigned nb_threads = atoi(item3->buffer);
    unsigned threshold = atoi(item4->buffer);
    struct mat *out = mult_mat_auto(in1, in2, nb_threads, threshold);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(in2);
    free_mat(out);
    free(res);
  }
  else if (!strcmp(argv[1], "div"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    float in2 = atof(item2->buffer);
    struct mat *out = div_mat(in1, in2);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(out);
    free(res);
  }
  else if (!strcmp(argv[1], "dii"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    float in2 = atof(item2->buffer);
    div_mat_ip(in1, in2);
    char *res = serialize_mat(in1);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free(res);
  }
  else if (!strcmp(argv[1], "scm"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    float in2 = atof(item2->buffer);
    struct mat *out = sc_mult_mat(in1, in2);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(in1);
    free_mat(out);
    free(res);
  }
  else if (!strcmp(argv[1], "tra"))
  {
    get_input(buffer);
    struct buffer *item1 = get_item(buffer->buffer, 0);
    if (!item1)
    {
      fprintf(stderr, "Invalid input format.\n");
      return 3;
    }
    struct mat *in1 = deserialize_mat(item1->buffer);
    struct mat *out = trans_mat(in1);
    if (!out)
    {
      fprintf(stderr, "Function error.\n");
      return 5;
    }
    char *res = serialize_mat(out);
    printf("%s\n", res);
    free_buffer(item1);
    free_mat(in1);
    free_mat(out);
    free(res);
  }
  else
  {
    printf("Invalide command.\n");
    return 4;
  }
  free_buffer(buffer);
/*
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
*/
  return 0;
}
