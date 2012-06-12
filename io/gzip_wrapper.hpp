/**
 * @file
 * @ingroup wrapper
 * @brief Wrappers for different means of loading a gzip file.
 *
 *   The "_orig" method uses the original loading method.
 *   The "_buff" method uses a static buffer (one per function call).
 *   The "_buff_g" method uses a global buffer (don't use if different threads call this function).
 *   The "_buff_c" method uses a custom (user-specified) buffer. 
 *   
 *   The "_orig*" method constantly realloc() the destination buffer and read directly into it, then
 *      realloc() once more to reduce it to its final size.
 *   The "_buff*" methods read into a static buffer and memcpy() it onto the destination buffer when
 *      it's full. The hope is that this reduces fragmentation when loading lots of small files, since
 *      _orig* will allocate 256k and then try to shrink it down to, say, 5k.
 *   If the "_buff_g" method detects that a different thread is accessing it, it will emit a warning.
 *       This does not necessarily imply a race condition; in this case you might create a thread-local
 *       buffer and pass it in to "_buff_c" yourself.
 */
#ifndef UUID_72e0d186_d437_46e9_b84b_413701fc701e
#define UUID_72e0d186_d437_46e9_b84b_413701fc701e

//Note: I couldn't get EL to compile WITHOUT faster startup.
//      I'm assuming this is an older flag?
#ifndef FASTER_STARTUP
#error Must use FASTER_STARTUP option.
#endif

#include "elfilewrapper.h"
#include "../xz/Types.h"
#include <zlib.h>

#ifdef __cplusplus
extern "C"
{
#endif

unsigned char* read_gzip_stream_orig(gzFile srcFile, Sint64* resSize);

unsigned char* read_gzip_stream_buff(gzFile srcFile, Sint64* size);

unsigned char* read_gzip_stream_buff_g(gzFile srcFile, Sint64* size);

unsigned char* read_gzip_stream_buff_c(unsigned char* chunk_buff, const Sint32 chunk_size, gzFile srcFile, Sint64* size);

#ifdef __cplusplus
}
#endif



#endif	/* UUID_72e0d186_d437_46e9_b84b_413701fc701e */

