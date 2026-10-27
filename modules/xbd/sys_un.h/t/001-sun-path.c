/*
 * sun_path is a char array of at least 92 bytes and the last member of
 *   sockaddr_un.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_un.h.001
 */
#include <sys/un.h>
#include <stddef.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	struct sockaddr_un u;
	size_t end = offsetof(struct sockaddr_un, sun_path) + sizeof u.sun_path;

	pc_begin("XBD.sys_un.h.001");
	pc_test();
	pc_check((sa_family_t)-1 > 0 && (sa_family_t)1.9 == 1, "sa_family_t is not an unsigned integer type");
	pc_check(IS_T(u.sun_family, sa_family_t), "sun_family does not have type sa_family_t");
	pc_check(IS_T(u.sun_path[0], char), "sun_path is not an array of char");
	pc_check(sizeof u.sun_path >= 92, "sun_path holds %lu bytes, fewer than 92", (unsigned long)sizeof u.sun_path);
	pc_check(offsetof(struct sockaddr_un, sun_path) >= offsetof(struct sockaddr_un, sun_family) + sizeof u.sun_family, "sun_path overlaps or precedes sun_family");
	pc_check(sizeof u - end < _Alignof(struct sockaddr_un), "sockaddr_un extends %lu bytes beyond sun_path", (unsigned long)(sizeof u - end));
	pc_pass();
}
