#include "misc_managers.hpp"

#include <set>

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
	std::set<PF_TILE *> all_tile_maps;

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



namespace {


MiscManager::~MiscManager() {
	MISC_MANAGER_ACTIVE = false;

	//Delete all items from all sets.
	free_all_elements(all_shirt_parts);
	free_all_elements(all_weapon_parts);
	free_all_elements(all_tile_maps);
}


} //End un-named namespace

