/*
 * msgqnum_t and msglen_t are unsigned integer types at least as large as
 * unsigned short.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_msg.h.002
 */
#include "pc.h"
#include <sys/msg.h>

int main(void)
{
	pc_begin("XBD.sys_msg.h.002");
	pc_test();
	pc_check((msgqnum_t)-1 > 0, "msgqnum_t is not an unsigned type");
	pc_check((msgqnum_t)0.5 == 0, "msgqnum_t is not an integer type");
	pc_check((msglen_t)-1 > 0, "msglen_t is not an unsigned type");
	pc_check((msglen_t)0.5 == 0, "msglen_t is not an integer type");
	pc_check(sizeof(msgqnum_t) >= sizeof(unsigned short), "msgqnum_t is smaller than unsigned short");
	pc_check(sizeof(msglen_t) >= sizeof(unsigned short), "msglen_t is smaller than unsigned short");
	pc_pass();
}
