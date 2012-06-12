#include "xml_wrapper.hpp"

#include "../elloggingwrapper.h"
#include "../actors.h" //Contains MAX_FILE_PATH
#include "../asc.h"    //Contains get_string_value()

namespace  {

const size_t StringBuffChunkSize = MAX_FILE_PATH+2; //+2 for some wiggle room
int last_pid = -1;
char global_buffer[StringBuffChunkSize];

//Helper function: called only when we know maxlen<buff
char* get_item_string_value_buff_ok(char* buf, size_t maxlen, const xmlNode *item, const unsigned char *name)
{
	char local_buffer[StringBuffChunkSize];
	return get_item_string_value_buff_c(&local_buffer[0], buf, maxlen, item, name);
}


//Helper function: called when we need to allocate a temporary buffer
char* get_item_string_value_buff_notok(char* buf, size_t maxlen, const xmlNode *item, const unsigned char *name)
{
	LOG_WARNING("WARNING: get_item_string_value called with longer buffer than MAX_FILE_PATH: %d\n", maxlen);
	char* chunk = new char[maxlen+1];
	char* res = get_item_string_value_buff_c(chunk, buf, maxlen, item, name);
	delete [] chunk;

	return res;
}

//Similar helper function for get_string_value_buff()
char* get_string_value_buff_ok(char* buf, size_t maxlen, const xmlNode* node)
{
	char local_buffer[StringBuffChunkSize];
	return get_string_value_buff_c(&local_buffer[0], buf, maxlen, node);
}


//Similar helper function for get_string_value_buff()
char* get_string_value_buff_notok(char* buf, size_t maxlen, const xmlNode* node)
{
	LOG_WARNING("WARNING: get_string_value called with longer buffer than MAX_FILE_PATH: %d\n", maxlen);
	char* chunk = new char[maxlen+1];
	char* res = get_string_value_buff_c(chunk, buf, maxlen, node);
	delete [] chunk;

	return res;
}


//Helper function: (re)allocate and copy over a string.
//We need to realloc() because a value may only be released once (even if
//  there are multiple attempts to load it).
//Note that this function is only ever called after something like strncpy();
//  this ensures that the trailing zero has been set (and thus strlen and strcpy will
//  work without error).
char* copy_over_string(char* dest, const char* const src) {
	const size_t size = strlen(src);
	dest = (char*)realloc(dest, (size+1)*sizeof(char));
	strcpy(dest, src);
	return dest;
}

} //End un-named namespace


extern "C" char* get_item_string_value_buff_c(char* chunk_buff, char* buf, size_t maxlen, const xmlNode *item, const unsigned char *name)
{
	//Sanity check.
	if (!item) {
		LOG_ERROR("Item is null!");
		return copy_over_string(buf, "\0");
	}

	//Assume chunk_buff has at least room for one character
	chunk_buff[0] = '\0';

	// look for this entry in the children
	for (const xmlNode* node=item->children; node; node=node->next) {
		if (node->type==XML_ELEMENT_NODE && xmlStrcasecmp(node->name, name)==0) {
			//Save it to the chunk buffer.
			get_string_value(chunk_buff, maxlen, node);
			break;
		}
	}

	//Copy and return the string.
	return copy_over_string(buf, chunk_buff);
}


extern "C" char* get_item_string_value_buff(char* buf, size_t maxlen, const xmlNode *item, const unsigned char *name)
{
	//Easy case
	if (maxlen < StringBuffChunkSize) {
		return get_item_string_value_buff_ok(buf, maxlen, item, name);
	}

	//Somewhat unexpected, but we can still handle it.
	return get_item_string_value_buff_notok(buf, maxlen, item, name);
}

extern "C" char* get_item_string_value_buff_g(char* buf, size_t maxlen, const xmlNode *item, const unsigned char *name)
{
	if (maxlen < StringBuffChunkSize) {
		//Minor safety valve in case multiple threads try to use this function.
		int curr_pid = getpid();
		if (last_pid == -1) {
			last_pid = curr_pid; //Should work on Windows too.
		} else if (last_pid != curr_pid) {
			LOG_WARNING("get_item_string_value called with from two different threads %x , %x\n", last_pid, curr_pid);
		}

		//Try it anyway; it might not actually be an error.
		return get_item_string_value_buff_c(&global_buffer[0], buf, maxlen, item, name);
	}

	//Somewhat unexpected, but we can still handle it.
	return get_item_string_value_buff_notok(buf, maxlen, item, name);
}



extern "C" char* get_string_value_buff(char* buf, size_t maxlen, const xmlNode* node)
{
	//Easy case
	if (maxlen < StringBuffChunkSize) {
		return get_string_value_buff_ok(buf, maxlen, node);
	}

	//Somewhat unexpected, but we can still handle it.
	return get_string_value_buff_notok(buf, maxlen, node);
}


extern "C" char* get_string_value_buff_g(char* buf, size_t maxlen, const xmlNode* node)
{
	if (maxlen < StringBuffChunkSize) {
		//Minor safety valve in case multiple threads try to use this function.
		int curr_pid = getpid();
		if (last_pid == -1) {
			last_pid = curr_pid; //Should work on Windows too.
		} else if (last_pid != curr_pid) {
			LOG_WARNING("get_string_value called with from two different threads %x , %x\n", last_pid, curr_pid);
		}

		//Try it anyway; it might not actually be an error.
		return get_string_value_buff_c(&global_buffer[0], buf, maxlen, node);
	}

	//Somewhat unexpected, but we can still handle it.
	return get_string_value_buff_notok(buf, maxlen, node);
}


extern "C" char* get_string_value_buff_c(char* chunk_buff, char* buf, size_t maxlen, const xmlNode* node)
{
	//Sanity checks
	if (!node) {
		LOG_ERROR("Node is null!");
		return copy_over_string(buf, "\0");
	}
	if (!node->children) {
		return copy_over_string(buf, "\0");
	}

	//Copy the string to our chunk buffer, then copy that into our result.
	my_strncp(chunk_buff, (const char*)node->children->content, maxlen);
	return copy_over_string(buf, chunk_buff);
}



