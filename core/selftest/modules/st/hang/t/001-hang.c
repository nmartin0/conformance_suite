/* Harness self-test. Copyright (c) 2026 Nicholas Martin. All rights reserved. */
#include "pc.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(void)
{
	const char *d = getenv("PC_SELFTEST_DIR");
	char path[1024];
	pc_begin("ST.hang.001");
	pc_test();
	if (fork() == 0) {
		/* If the timeout fails to kill us, leave a marker. */
		sleep(2);
		if (d) {
			(void)snprintf(path, sizeof path, "%s/hang-child", d);
			(void)close(open(path, O_WRONLY | O_CREAT, 0644));
		}
		_exit(0);
	}
	for (;;)
		pause();
}
