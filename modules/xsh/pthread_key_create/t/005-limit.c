/*
 * pthread_key_create() fails rather than exceed {PTHREAD_KEYS_MAX}.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.pthread_key_create.005
 * MUTANT    pthread_key_create.limit_einval
 * VARIANT   pthread_key_create.keys_max_128
 * VARIANT   pthread_key_create.keys_enomem
 */
#include "pc.h"
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
	long max = sysconf(_SC_THREAD_KEYS_MAX), i, made = 0, at = -1;
	pthread_key_t *k;
	int r, first = 0;

	pc_begin("XSH.pthread_key_create.005");
#ifdef PTHREAD_KEYS_MAX
	if (max < 0)
		max = PTHREAD_KEYS_MAX;
#endif
	if (max < 0)
		pc_notinuse("{PTHREAD_KEYS_MAX} is indeterminate");
	if (max > 65536)
		pc_notinuse("{PTHREAD_KEYS_MAX} is %ld, too many keys to create here", max);
	k = malloc(sizeof *k * (size_t)(max + 1));
	pc_check(k != NULL, "malloc failed");
	pc_test();
	for (i = 0; i <= max; i++) {
		r = pthread_key_create(&k[made], NULL);
		if (r != 0) {
			first = r;
			at = i;
			break;
		}
		made++;
	}
	for (i = 0; i < made; i++)
		(void)pthread_key_delete(k[i]);
	pc_check(at >= 0, "%ld keys were created, more than {PTHREAD_KEYS_MAX} (%ld)", made, max);
	pc_note("creation %ld of %ld failed", at + 1, max + 1);
	pc_check(first == EAGAIN || first == ENOMEM, "the failing pthread_key_create returned %s, expected EAGAIN or ENOMEM",
		 pc_errname(first));
	pc_pass();
}
