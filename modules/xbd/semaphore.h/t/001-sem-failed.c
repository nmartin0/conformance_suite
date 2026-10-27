/*
 * SEM_FAILED has type sem_t *.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.semaphore.h.001
 */
#include "pc.h"
#include <semaphore.h>

int main(void)
{
	pc_begin("XBD.semaphore.h.001");
#ifndef SEM_FAILED
	pc_unresolved("SEM_FAILED is missing (the header check reports it)");
#else
	pc_test();
	pc_check(_Generic((SEM_FAILED), sem_t *: 1, default: 0), "SEM_FAILED does not have type sem_t *");
	pc_pass();
#endif
}
