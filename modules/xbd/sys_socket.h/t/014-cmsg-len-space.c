/*
 * CMSG_LEN and CMSG_SPACE are constant expressions that leave room for
 *   the header and data; CMSG_DATA points into the object.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.014
 */
#include <sys/socket.h>
#include <string.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

/* Room for two small objects, aligned for a struct cmsghdr. */
static union {
	struct cmsghdr align;
	unsigned char buf[2 * CMSG_SPACE(sizeof(int)) + 64];
} u;

enum { const_len = CMSG_LEN(sizeof(int)), const_space = CMSG_SPACE(sizeof(int)) };

int main(void)
{
	static const size_t lens[] = { 0, 1, 3, 4, 8, 13, 64 };
	struct cmsghdr *c = (struct cmsghdr *)u.buf;
	size_t i;

	pc_begin("XBD.sys_socket.h.014");
	pc_test();
	pc_check(const_len > 0 && const_space >= const_len, "CMSG_LEN(sizeof(int)) is %d and CMSG_SPACE(sizeof(int)) is %d", (int)const_len, (int)const_space);
	pc_check(IS_T(CMSG_DATA(c), unsigned char *), "CMSG_DATA does not have type unsigned char *");
	for (i = 0; i < sizeof lens / sizeof lens[0]; i++) {
		size_t n = lens[i], len = CMSG_LEN(n), space = CMSG_SPACE(n), off;

		pc_check(len >= sizeof(struct cmsghdr) + n, "CMSG_LEN(%lu) is %lu, less than the header plus the data", (unsigned long)n, (unsigned long)len);
		pc_check(space >= len, "CMSG_SPACE(%lu) is %lu, less than CMSG_LEN(%lu) (%lu)", (unsigned long)n, (unsigned long)space, (unsigned long)n, (unsigned long)len);
		pc_check(space % _Alignof(struct cmsghdr) == 0, "CMSG_SPACE(%lu) is %lu, not a multiple of the alignment of struct cmsghdr", (unsigned long)n, (unsigned long)space);
		off = (size_t)((unsigned char *)CMSG_DATA(c) - (unsigned char *)c);
		pc_check(off >= sizeof(struct cmsghdr), "CMSG_DATA is at offset %lu, inside the header", (unsigned long)off);
		pc_check(off + n <= len, "%lu data bytes at offset %lu do not fit in CMSG_LEN(%lu) = %lu", (unsigned long)n, (unsigned long)off, (unsigned long)n, (unsigned long)len);
	}
	pc_pass();
}
