#include "queue_manager.hpp"

#include <map>

#include "elloggingwrapper.h"

namespace  {

//Is the queue manager active?
bool QUEUE_MANAGER_ACTIVE = true;

//Helper class
class QueueManager {
public:
	//Flag is "active"; setting this is faster then deleting items (because most structs clean up
	//  their own queues). 
	std::map<queue_t*, bool> all_queues;
	~QueueManager();
}; 

//Holds all managed queues
QueueManager manager;

} //End un-named namespace


extern "C" void begin_managing_queue(queue_t* q)
{
	if (!QUEUE_MANAGER_ACTIVE) { return; }

	std::map<queue_t*, bool>::iterator it = manager.all_queues.find(q);
	if (it!=manager.all_queues.end() && it->second) {
		LOG_WARNING("Queue is already being managed.");
	} else {
		manager.all_queues[q] = true;
	}
}

extern "C" void stop_managing_queue(queue_t* q) 
{
	if (!QUEUE_MANAGER_ACTIVE) { return; }

	std::map<queue_t*, bool>::iterator it = manager.all_queues.find(q);
	if (it==manager.all_queues.end() || (!it->second)) {
		LOG_WARNING("Queue is not managed; can't remove it.");
	} else {
		manager.all_queues[q] = false;
		//manager.all_queues.erase(it);
	}
}


namespace {


QueueManager::~QueueManager() {
	QUEUE_MANAGER_ACTIVE = false;
	for (std::map<queue_t*, bool>::iterator it=all_queues.begin(); it!=all_queues.end(); it++) {
		if (it->second) {
			queue_destroy(it->first);
		}
	}
}


} //End un-named namespace

