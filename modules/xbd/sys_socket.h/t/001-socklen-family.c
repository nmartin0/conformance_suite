/*
 * socklen_t is an integer type of at least 32 bits; sa_family_t is an
 *   unsigned integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.001
 */
#include <sys/socket.h>
#include <limits.h>
#include "pc.h"


int main(void)
{
	pc_begin("XBD.sys_socket.h.001");
	pc_test();
	pc_check((socklen_t)1.9 == 1, "socklen_t is not an integer type");
	pc_check(sizeof(socklen_t) * CHAR_BIT >= 32, "socklen_t is narrower than 32 bits");
	pc_check((sa_family_t)1.9 == 1, "sa_family_t is not an integer type");
	pc_check((sa_family_t)-1 > 0, "sa_family_t is not unsigned");
	pc_pass();
}
