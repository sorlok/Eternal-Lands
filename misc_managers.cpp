#include "misc_managers.hpp"

#include <set>

#include "cal3d_wrapper.h"
#include "elloggingwrapper.h"
#include "cal3d/cal3d.h"

namespace  {

//Is insertion/deletion allowed?
bool MISC_MANAGER_ACTIVE = true;

//Helper class
class MiscManager {
public:
	//Sets for each manageable item. You only need a new set if there's a custom destructor 
	//  (or if you're using delete/delete[] instead of free)
	std::set<void*> all_voidps;
	std::set<CalCoreModel*> all_cal_core_models;
	std::set<cache_struct*> all_caches;

	~MiscManager();
}; 

//Holds all managed items
MiscManager manager;


//Helper script: begin managing<>
template <typename ItemType>
void begin_managing(std::set<ItemType*>& dest, ItemType*& it, const char* const err_name)
{
	if (!MISC_MANAGER_ACTIVE) { return; }
	if (!it) { return; }
	if (dest.find(it) != dest.end()) {
		LOG_WARNING("%s is already being managed.", err_name);
	} else {
		dest.insert(it);
	}
}

//Helper script: stop managing<>
template <typename ItemType>
void stop_managing(std::set<ItemType*>& dest, ItemType*& it, const char* const err_name)
{
	if (!MISC_MANAGER_ACTIVE) { return; }
	if (!it) { return; }
	if (dest.find(it) == dest.end()) {
		LOG_WARNING("%s is not being managed.", err_name);
	} else {
		dest.erase(it);
	}
}

//Helper script: free all elements<>
template <typename ItemType>
void free_all_elements(const std::set<ItemType*>& elems)
{
	for (typename std::set<ItemType*>::const_iterator it=elems.begin(); it!=elems.end(); it++) {
		if (*it) {
			free(*it);
		}
	}
}


//CalCoreModels are special
void delete_cal_core_model(CalCoreModel* model)
{
	//Note: It appears that CalCoreModels delete their animations properly... so where else are we storing them?
	int size = CalCoreModel_GetCoreAnimationCount(model);
	for (int i=0; i<size; i++) {
		model->unloadCoreAnimation(i);
	}

	//Finally
	CalCoreModel_Delete(model);
}



} //End un-named namespace


extern "C" void begin_managing_memchunk(void* vp)
{
	begin_managing(manager.all_voidps, vp, "void*");
}

extern "C" void stop_managing_memchunk(void* vp)
{
	stop_managing(manager.all_voidps, vp, "void*");
}

extern "C" void begin_managing_cal_core_model(struct CalCoreModel* cm)
{
	begin_managing(manager.all_cal_core_models, cm, "Cal Core Model");
}

extern "C" void stop_managing_cal_core_model(struct CalCoreModel* cm)
{
	stop_managing(manager.all_cal_core_models, cm, "Cal Core Model");
}

extern "C" int is_managing_cal_core_model(struct CalCoreModel* cm)
{
	return manager.all_cal_core_models.count(cm);
}

extern "C" void begin_managing_cache(cache_struct* cache)
{
	begin_managing(manager.all_caches, cache, "Cache");
}

extern "C" void stop_managing_cache(cache_struct* cache)
{
	stop_managing(manager.all_caches, cache, "Cache");
}


namespace {


MiscManager::~MiscManager() {
	MISC_MANAGER_ACTIVE = false;

	//Delete all items from all sets.
	free_all_elements(all_voidps);

	//CalCoreModels are hefty; we should probably delete these all by hand.
	{
	std::set<CalCoreModel*>::const_iterator it=all_cal_core_models.begin();
	for (; it!=all_cal_core_models.end(); it++) {
		if (*it) {
			delete_cal_core_model(*it);
		}
	}
	}

	//Caches also have their own delete function
	{
	std::set<cache_struct*>::const_iterator it=all_caches.begin();
	for (; it!=all_caches.end(); it++) {
		if (*it) {
			cache_delete(*it);
		}
	}
	}
}


} //End un-named namespace

