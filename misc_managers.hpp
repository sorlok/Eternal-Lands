/**
 * @file
 * @ingroup misc
 * @brief Allows easy management of miscellaneous items.
 *
 *   The misc managers are similar to the queue_manager, but 
 *   far less powerful. They are meant for quick fixes to memory 
 *   management issues (e.g., actor->shirt is never cleaned up)
 *   and do not allow lookup and (possible bad performance for) removal.
 *   It is expected that eventually C++ objects will manage their own
 *   items using RAII classes, rather than relying on external managers
 *   such as the misc managers, but for C objects this is the most
 *   auotmatic alternative.
 */
#ifndef UUID_e0e606f9_66e1_4931_a887_ee6fe85e21de
#define UUID_e0e606f9_66e1_4931_a887_ee6fe85e21de

#ifdef __cplusplus
extern "C"
{
#endif

#include "actors.h"
#include "pathfinder.h"

//The easy way
struct CalCoreModel;

void begin_managing_shirt_part(shirt_part* sp);
void begin_managing_weapon_part(weapon_part* wp);
void begin_managing_tile_map(PF_TILE * tm);
void stop_managing_tile_map(PF_TILE * tm);
void begin_managing_voidp(void* vp);
void stop_managing_voidp(void* vp);
void begin_managing_cal_core_model(struct CalCoreModel* cm);

#ifdef __cplusplus
}
#endif



#endif	/* UUID_e0e606f9_66e1_4931_a887_ee6fe85e21de */

