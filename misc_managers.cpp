#include "misc_managers.hpp"

#include <set>

#include "cal3d_wrapper.h"
#include "elloggingwrapper.h"

namespace  {

//Is insertion/deletion allowed?
bool MISC_MANAGER_ACTIVE = true;

//Helper class
class MiscManager {
public:
	//Sets for each manageable item. We never remove items, so a set is sufficient.
	std::set<shirt_part*> all_shirt_parts;
	std::set<weapon_part*> all_weapon_parts;
	std::set<PF_TILE*> all_tile_maps;
	std::set<void*> all_voidps; //Don't overuse this one.
	std::set<CalCoreModel*> all_cal_core_models;

	~MiscManager();
}; 

//Holds all managed items
MiscManager manager;


//Helper script: begin managing<>
template <typename Item>
void begin_managing(std::set<Item*>& dest, Item* it, const char* const err_name)
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
template <typename Item>
void stop_managing(std::set<Item*>& dest, Item* it, const char* const err_name)
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
template <typename Item>
void free_all_elements(const std::set<Item*>& elems)
{
	for (typename std::set<Item*>::const_iterator it=elems.begin(); it!=elems.end(); it++) {
		if (*it) {
			free(*it);
		}
	}
}


//CalCoreModels are special
void delete_cal_core_model(CalCoreModel* model)
{
	//Clear all meshes. NOTE: It seems CalCoreModel_Delete does this automatically.
/*	int size = CalCoreModel_GetCoreMeshCount(model);
	for (int i=0; i<size; i++) {
		CalCoreMesh* mesh = CalCoreModel_GetCoreMesh(model, i);
		CalCoreMesh_Delete(mesh);
	}*/

	//Finally
	CalCoreModel_Delete(model);
}



} //End un-named namespace


extern "C" void begin_managing_shirt_part(shirt_part* sp)
{
	begin_managing(manager.all_shirt_parts, sp, "Shirt part");
}

extern "C" void begin_managing_weapon_part(weapon_part* wp)
{
	begin_managing(manager.all_weapon_parts, wp, "Weapon part");
}

extern "C" void begin_managing_tile_map(PF_TILE* tm)
{
	begin_managing(manager.all_tile_maps, tm, "Tile map");
}

extern "C" void stop_managing_tile_map(PF_TILE* tm)
{
	stop_managing(manager.all_tile_maps, tm, "Tile map");
}

extern "C" void begin_managing_voidp(void* vp)
{
	begin_managing(manager.all_voidps, vp, "void*");
}

extern "C" void stop_managing_voidp(void* vp)
{
	stop_managing(manager.all_voidps, vp, "void*");
}

extern "C" void begin_managing_cal_core_model(CalCoreModel* cm)
{
	begin_managing(manager.all_cal_core_models, cm, "Cal Core Model");
}

namespace {


MiscManager::~MiscManager() {
	MISC_MANAGER_ACTIVE = false;

	//Delete all items from all sets.
	free_all_elements(all_shirt_parts);
	free_all_elements(all_weapon_parts);
	free_all_elements(all_tile_maps);
	free_all_elements(all_voidps);

	//CalCoreModels are hefty; we should probably delete these all by hand.
	std::set<CalCoreModel*>::const_iterator it=all_cal_core_models.begin();
	for (; it!=all_cal_core_models.end(); it++) {
		if (*it) {
			delete_cal_core_model(*it);
		}
	}
}


} //End un-named namespace

