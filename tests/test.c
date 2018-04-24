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

int main(int argc, char *argv[])
{
  if (argc != 2)
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
    printf("  +---------+-----------------+------------+--------------+\n");
    printf("  | Command | Function        | stdin      | stdout       |\n");
    printf("  +---------+-----------------+------------+--------------+\n");
    printf("  | dec     | dec_mat()       | [num;num;] | 1 matrix     |\n");
    printf("  | ini     | init_mat()      | [num;num;] | 1 matrix     |\n");
    printf("  | cpy     | copy_mat()      | 1 matrix   | 1 matrix     |\n");
    printf("  | ieq     | is_equal()      | 2 matrices | [True|False] |\n");
    printf("  | add     | add_mat()       | 2 matrices | 1 matrix     |\n");
    printf("  | adi     | add_mat_ip()    | 2 matrices | 1 matrix     |\n");
    printf("  | sub     | sub_mat()       | 2 matrices | 1 matrix     |\n");
    printf("  | sui     | sub_mat_ip()    | 2 matrices | 1 matrix     |\n");
    printf("  | mul     | mult_mat()      | 2 matrices | 1 matrix     |\n");
    printf("  | mut     | mult_mat_th()   | 2 matrices | 1 matrix     |\n");
    printf("  | mua     | mult_mat_auto() | 2 matrices | 1 matrix     |\n");
    printf("  | div     | div_mat()       | 1 matrix   | 1 matrix     |\n");
    printf("  | dii     | div_mat_ip()    | 1 matrix   | 1 matrix     |\n");
    printf("  | scm     | sc_mult_mat()   | 1 matrix   | 1 matrix     |\n");
    printf("  | tra     | trans_mat()     | 1 matrix   | 1 matrix     |\n");
    printf("  +---------+-----------------+------------+--------------+\n");
    printf("\n");
    printf("Return values :\n");
    printf(" 0 -> Everything whent well.\n");
    printf(" 1 -> Not a valid command.\n");
    printf(" 2 -> Memory allocation problems.\n");
    printf(" 3 -> Invalid input (parsing error).\n");
    printf("\n");
    return 0;
  }

  struct buffer *buffer = init_buffer();
  char c = 0;

  if (!buffer)
  {
    perror("init_buffer: Unable to create buffer");
    return 2;
  }

  if (!strcmp(argv[1], "dec"))
  {
    c = getchar();
    while (c != EOF && c != '\n')
    {
      add_buffer(buffer, &c, 1);
      c = getchar();
    }
    struct buffer *item1 = get_item(buffer->buffer, 0);
    struct buffer *item2 = get_item(buffer->buffer, 1);
    if (!item1 || !item2)
    {
      fprintf(stderr, "Invalide input format.");
      return 3;
    }
    size_t num1 = atoi(item1->buffer);
    size_t num2 = atoi(item2->buffer);
    struct mat *a = dec_mat(num1, num2);
    char *res = serialize_mat(a);
    printf("%s", res);
    free_buffer(item1);
    free_buffer(item2);
    free_mat(a);
    free(res);
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
