/*
 * A name mq_open() accepted is not refused by mq_unlink() as too long.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.mq_unlink.005
 */
#include "mqh.h"

int main(void)
{
	static const int lens[] = { 4200, 1500, 255, 100 };
	static char nm[4201];
	struct mq_attr at;
	mqd_t q = BADQ;
	size_t i;
	char pre[32];
	int e, r;

	pc_begin("XSH.mq_unlink.005");
	snprintf(pre, sizeof pre, "/pcmq%ldx", (long)getpid());
	memset(&at, 0, sizeof at);
	at.mq_maxmsg = 4;
	at.mq_msgsize = 64;
	for (i = 0; i < sizeof lens / sizeof lens[0]; i++) {
		/* Unique per process, so a leftover or a parallel run cannot collide. */
		memset(nm, 'b', (size_t)lens[i]);
		nm[lens[i]] = '\0';
		memcpy(nm, pre, strlen(pre));
		q = mq_open(nm, O_RDWR | O_CREAT | O_EXCL, (mode_t)0600, &at);
		if (q != BADQ)
			break;
	}
	NEED(q != BADQ, "no long name was accepted by mq_open: %s", pc_errname(errno));
	(void)mq_close(q);
	pc_test();
	errno = 0;
	r = mq_unlink(nm);
	e = errno;
	pc_check(r == 0 || e != ENAMETOOLONG, "mq_unlink gave ENAMETOOLONG for a name of %d bytes that mq_open accepted", lens[i]);
	pc_check(r == 0, "mq_unlink of a name of %d bytes failed: %s", lens[i], pc_errname(e));
	pc_pass();
}
