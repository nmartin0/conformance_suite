/*
 * pthread_mutex_init.c - conforming variants of pthread_mutex_destroy(),
 * for the variant checks (CI only).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * A preload library; see core/pcvariant.h. Each variant says why the
 * standard allows it.
 */
#define _GNU_SOURCE 1
#include <pthread.h>
#include "pcvariant.h"

/*
 * "destroy_poisons": a successful pthread_mutex_destroy() fills the
 * object with an invalid bit pattern. The page allows pthread_mutex_destroy()
 * to set the object to an invalid value; pthread_mutex_init() must still
 * reinitialize it.
 */
int pthread_mutex_destroy(pthread_mutex_t *mx)
{
	const char *v = variant_of("pthread_mutex_init.");
	int r;
	REAL(pthread_mutex_destroy);

	r = real_pthread_mutex_destroy(mx);
	if (r == 0 && IS("destroy_poisons"))
		memset(mx, 0xa5, sizeof *mx);
	return r;
}
