/**
** \file buffer.h
*/

#ifndef BUFFER_H
# define BUFFER_H

# include <stddef.h>

/**
** buffer
**/
struct buffer
{
  size_t size; /**< Size of the buffer. */
  char *buffer; /**< Array representing the buffer. */
};

/**
** \fn struct buffer *init_buffer(void)
** \brief Init and allocat a buffer to size = 0 and buffer = NULL.
** \return Returns an initialized an allocated buffer structure.
*/
struct buffer *init_buffer(void);

/**
** \fn void add_buffer(struct buffer *buffer, const char *str, size_t size)
** \brief Concatenat a string to a buffer structure.
** \param buffer The buffer structure.
** \param str The string to concatenat to the buffer.
** \param size Size of the string str.
*/
void add_buffer(struct buffer *buffer, const char *str, size_t size);

/**
** \fn void reduce_buffer(struct buffer *buffer, size_t new_size)
** \brief Reduce the size of a buffer.
** \param buffer The buffer to reduce.
** \param new_size The expected new size of the buffer.
*/
void reduce_buffer(struct buffer *buffer, size_t new_size);

/**
** \fn void empty_buffer(struct buffer *buffer)
** \brief Empty a buffer structure but not the struct itself.
** \param buffer The buffer to empty.
*/
void empty_buffer(struct buffer *buffer);

/**
** \fn void free_buffer(struct buffer *buffer)
** \brief Free completely a buffer.
** \param buffer The buffer to free.
*/
void free_buffer(struct buffer *buffer);

#endif /* !struct buffer */
