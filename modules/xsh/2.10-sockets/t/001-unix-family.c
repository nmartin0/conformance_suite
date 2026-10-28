/*
 * The names of AF_UNIX sockets carry the family AF_UNIX in sa_family.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.001
 */
#include "pcsk.h"

int main(void)
{
	struct sockaddr_un a, got;
	struct sockaddr_storage ss;
	socklen_t al, gl;
	int l, c, k;

	pc_begin("XSH.2.10-sockets.001");
	guard(60);
	l = socket(AF_UNIX, SOCK_STREAM, 0);
	c = socket(AF_UNIX, SOCK_STREAM, 0);
	NEED(l != -1 && c != -1, "socket failed: %s", pc_errname(errno));
	al = unix_name(&a, "u1");
	(void)unlink("u1");
	NEED(bind(l, (struct sockaddr *)&a, al) == 0, "bind failed: %s", pc_errname(errno));
	NEED(listen(l, 2) == 0, "listen failed: %s", pc_errname(errno));
	NEED(connect(c, (struct sockaddr *)&a, al) == 0, "connect failed: %s", pc_errname(errno));
	pc_test();

	memset(&ss, 0, sizeof ss);
	gl = sizeof ss;
	k = getsockname(l, (struct sockaddr *)&ss, &gl);
	pc_check(k == 0, "getsockname() of a bound AF_UNIX socket failed: %s", pc_errname(errno));
	pc_check(gl >= offsetof(struct sockaddr, sa_family) + sizeof ss.ss_family, "getsockname() length %lu is too short to hold the family", (unsigned long)gl);
	pc_check(((struct sockaddr *)&ss)->sa_family == AF_UNIX, "the family in getsockname() of a bound AF_UNIX socket is not AF_UNIX");
	memcpy(&got, &ss, sizeof got);
	pc_check(got.sun_family == AF_UNIX, "sun_family of the stored name is not AF_UNIX");
	pc_check(gl > offsetof(struct sockaddr_un, sun_path) && strcmp(got.sun_path, "u1") == 0, "the stored name is not the name bound");

	memset(&ss, 0, sizeof ss);
	gl = sizeof ss;
	k = getpeername(c, (struct sockaddr *)&ss, &gl);
	pc_check(k == 0, "getpeername() of a connected AF_UNIX socket failed: %s", pc_errname(errno));
	pc_check(((struct sockaddr *)&ss)->sa_family == AF_UNIX, "the family in getpeername() of a connected AF_UNIX socket is not AF_UNIX");
	(void)unlink("u1");
	pc_pass();
}
