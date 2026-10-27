/*
 * shmatt_t is an unsigned integer type at least as large as unsigned
 * short.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_shm.h.004
 */
#include "pc.h"
#include <sys/shm.h>

int main(void)
{
	pc_begin("XBD.sys_shm.h.004");
	pc_test();
	pc_check((shmatt_t)-1 > 0, "shmatt_t is not an unsigned type");
	pc_check((shmatt_t)0.5 == 0, "shmatt_t is not an integer type");
	pc_check(sizeof(shmatt_t) >= sizeof(unsigned short), "shmatt_t is smaller than unsigned short");
	pc_pass();
}
