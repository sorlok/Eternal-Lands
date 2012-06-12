/**
 * @file
 * @ingroup wrapper
 * @brief Wrappers for different means of loading XML data.
 *
 *   Several of the actors.h structs used "char item[MAX_PATH]" to 
 *    store paths (which are never written more than once). This makes freeing
 *    memory easier, but it has two consequences:
 *    1) Anything less than MAX_PATH wastes memory.
 *    2) As a result, MAX_PATH is usually set to a small value, limiting the size of 
 *       our file names.
 *
 *   This file provides a few functions, similar in behavior to the gzip_wrapper.hpp functions.
 *   The _buff function uses an internal buffer (creating it if the requested path size is actually
 *     bigger than MAX_PATH), and the _buff_g function uses a global buffer (checking if multiple threads
 *     are calling this function, which may be an error). _buff_c can be used with a char[] put on 
 *     thread-local storage to avoid the second issue.
 */
#ifndef UUID_bd89a5aa_df9b_4a4f_852d_1c24eeea5cce
#define UUID_bd89a5aa_df9b_4a4f_852d_1c24eeea5cce

#include <libxml/parser.h>

#include "../pawn/osdefs.h"

#ifdef __cplusplus
extern "C"
{
#endif

///get_item_string_value() overrides
char* get_item_string_value_buff(char* buf, size_t maxlen, const xmlNode* item, const unsigned char* name);
char* get_item_string_value_buff_g(char* buf, size_t maxlen, const xmlNode* item, const unsigned char* name);
char* get_item_string_value_buff_c(char* chunk_buff, char* buf, size_t maxlen, const xmlNode* item, const unsigned char* name);
///Note: NO check is performed on chunk_buff; make sure it can support a read of maxlen if you call this
//  function directly.


///get_string_value() overrides
char* get_string_value_buff(char* buf, size_t maxlen, const xmlNode* node);
char* get_string_value_buff_g(char* buf, size_t maxlen, const xmlNode* node);
char* get_string_value_buff_c(char* chunk_buff, char* buf, size_t maxlen, const xmlNode* node);
///Note: NO check is performed on chunk_buff; make sure it can support a read of maxlen if you call this
//  function directly.


#ifdef __cplusplus
}
#endif



#endif	/* UUID_bd89a5aa_df9b_4a4f_852d_1c24eeea5cce */

