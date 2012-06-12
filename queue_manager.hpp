/**
 * @file
 * @ingroup misc
 * @brief Allows easy management of queues.
 *
 *   This is used (by queue.h) to hold a reference to all active queues.
 *   It uses RAII to free those queues upon deletion.
 */
#ifndef UUID_57a40eae_61c9_4d76_bf64_fa419c07be80
#define UUID_57a40eae_61c9_4d76_bf64_fa419c07be80

#ifdef __cplusplus
extern "C"
{
#endif

#include "queue.h"

void begin_managing_queue(queue_t* q);

void stop_managing_queue(queue_t* q);

#ifdef __cplusplus
}
#endif



#endif	/* UUID_57a40eae_61c9_4d76_bf64_fa419c07be80 */

