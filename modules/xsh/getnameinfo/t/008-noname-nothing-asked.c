/*
 * Asking for neither node nor service fails with EAI_NONAME.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.getnameinfo.008
 */
#include "ni.h"

int main(void)
{
	static const unsigned char a[4] = { 127, 0, 0, 1 };
	struct sockaddr_in s = mk4(a, 80);
	char buf[16], buf2[16];
	int rc;

	pc_begin("XSH.getnameinfo.008");
	pc_test();
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, NULL, 0, NULL, 0, NUMF);
	pc_check(rc == EAI_NONAME, "no node and no service gave %d (%s), expected EAI_NONAME", rc, gai_strerror(rc));
	rc = getnameinfo((struct sockaddr *)&s, sizeof s, buf, 0, buf2, 0, NUMF);
	pc_check(rc == EAI_NONAME, "zero lengths gave %d (%s), expected EAI_NONAME", rc, gai_strerror(rc));
	pc_pass();
}
