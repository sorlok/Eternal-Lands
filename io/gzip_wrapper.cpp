#include "gzip_wrapper.hpp"

#include "../elloggingwrapper.h"

namespace  {
	const Sint32 GzipChunkSize = 0x40000;
	Sint64 last_pid = -1;
	unsigned char global_buffer[GzipChunkSize];
} //End un-named namespace


extern "C" unsigned char* read_gzip_stream_orig(gzFile srcFile, Sint64* resSize)
{
	//Convenience
	unsigned char* resBuffer = NULL;
	Sint64& size = *resSize;
	size = 0;
	
	//Keep reading until the "done" flag is set, or until we read less than 
	//the requested amount.
	bool done = false;
	while (!done) {
		resBuffer = (unsigned char*)(realloc(resBuffer, size+GzipChunkSize));
		Sint64 read = gzread(srcFile, resBuffer+size, GzipChunkSize);
		size += read;
		done = gzeof(srcFile) || read<GzipChunkSize;
	}

	//Resize the buffer to its smallest-possible valid size.
	resBuffer = (unsigned char*)realloc(resBuffer, size);
	return resBuffer;
}

extern "C" unsigned char* read_gzip_stream_buff(gzFile srcFile, Sint64* resSize)
{
	unsigned char stack_buffer[GzipChunkSize];
	return read_gzip_stream_buff_c(&stack_buffer[0], GzipChunkSize, srcFile, resSize);
}

extern "C" unsigned char* read_gzip_stream_buff_g(gzFile srcFile, Sint64* resSize)
{
	//Minor safety valve in case multiple threads try to use this function.
	Sint64 curr_pid = getpid();
	if (last_pid == -1) {
		last_pid = curr_pid; //Should work on Windows too.
	} else if (last_pid != curr_pid) {
		LOG_WARNING("read_gzip_stream called with from two different threads %x , %x\n", last_pid, curr_pid);
	}

	return read_gzip_stream_buff_c(&global_buffer[0], GzipChunkSize, srcFile, resSize);
}

extern "C" unsigned char* read_gzip_stream_buff_c(unsigned char* chunk_buff, const Sint32 chunk_size, gzFile srcFile, Sint64* resSize)
{
	//Convenience
	unsigned char* resBuffer = NULL;
	Sint64& size = *resSize;
	size = 0;

	//Keep reading until the "done" flag is set, or we read less than requested.
	bool done = false;
	while (!done) {
		//Read
		Sint64 read = gzread(srcFile, chunk_buff, chunk_size);
		done = gzeof(srcFile) || read<GzipChunkSize;

		//Append the block we just read. For small files, this is the ONLY time
		//memory is allocated. For large files, there is no need to shrink them at the end.
		resBuffer = (unsigned char*)(realloc(resBuffer, size+read));
		memcpy(resBuffer+size, chunk_buff, read);

		//Increment
		size += read;
	}
	return resBuffer;
}

