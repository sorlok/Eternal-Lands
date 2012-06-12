/**
 * @file
 * @ingroup misc
 * @brief Holds the set of all books.
 *
 *   This holds the set of all books in memory. There are two advantages
 *   to using this over the book* pointer used previously:
 *     1) The bookshelf uses RAII to ensure that all memory is cleaned up
 *        upon program exit (makes valgrind spew out less errors).
 *     2) The bookshelf uses a "left" and "right" set of adjoining books.
 *        This allows for faster binary lookup. It does NOT perform balancing,
 *        but this (a) can be done if needed and (b) in the worst case degrades
 *        only to linear time lookup, which is what we had before anyway.
 */
#ifndef UUID_145386ce_26ac_421f_9e63_bfe9621f1fe5
#define UUID_145386ce_26ac_421f_9e63_bfe9621f1fe5

#ifdef __cplusplus
extern "C"
{
#endif

#include "books.h"

//Forward declaration.
//struct book;

void add_book(book* newBook);

book* get_book(int id);

//void remove_book(book* oldBook); //Shouldn't be too hard to add.

#ifdef __cplusplus
}
#endif



#endif	/* UUID_145386ce_26ac_421f_9e63_bfe9621f1fe5 */

