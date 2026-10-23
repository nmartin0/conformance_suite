/*
 * <fcntl.h> defines F_GETOWN_EX, F_SETOWN_EX, F_OWNER_PID, F_OWNER_PGRP
 * and struct f_owner_ex with members type and pid.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.fcntl.h.008
 */
#include "pc.h"
#include <fcntl.h>

int main(void)
{
	pc_begin("XBD.fcntl.h.008");
	pc_test();
#if !defined(F_GETOWN_EX) || !defined(F_SETOWN_EX)
	pc_fail("<fcntl.h> does not define F_GETOWN_EX and F_SETOWN_EX");
#elif !defined(F_OWNER_PID) || !defined(F_OWNER_PGRP)
	pc_fail("<fcntl.h> does not define F_OWNER_PID and F_OWNER_PGRP as macros");
#else
	{
		struct f_owner_ex o;
		o.type = F_OWNER_PID;
		o.pid = 0;
		(void)o;
	}
	pc_check(F_OWNER_PID != F_OWNER_PGRP, "F_OWNER_PID and F_OWNER_PGRP are equal");
	pc_pass();
#endif
}
