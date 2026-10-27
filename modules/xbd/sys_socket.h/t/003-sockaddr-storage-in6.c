/*
 * sockaddr_storage is large and aligned enough for sockaddr_in6, with
 *   ss_family where sin6_family is.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.003
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stddef.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	struct sockaddr_storage ss;

	pc_begin("XBD.sys_socket.h.003");
	pc_test();
	pc_check(IS_T(ss.ss_family, sa_family_t), "ss_family does not have type sa_family_t");
	{
		struct sockaddr p;

		pc_check(IS_T(p.sa_family, sa_family_t), "sa_family does not have type sa_family_t");
		pc_check(offsetof(struct sockaddr_storage, ss_family) == offsetof(struct sockaddr, sa_family), "ss_family and sa_family are at different offsets");
		pc_check(_Alignof(struct sockaddr_storage) % _Alignof(struct sockaddr) == 0, "sockaddr_storage is aligned less strictly than struct sockaddr");
		(void)p;
	}
	{
		struct sockaddr_in6 p;

		pc_check(IS_T(p.sin6_family, sa_family_t), "sin6_family does not have type sa_family_t");
		pc_check(offsetof(struct sockaddr_storage, ss_family) == offsetof(struct sockaddr_in6, sin6_family), "ss_family and sin6_family are at different offsets");
		pc_check(_Alignof(struct sockaddr_storage) % _Alignof(struct sockaddr_in6) == 0, "sockaddr_storage is aligned less strictly than struct sockaddr_in6");
		pc_check(sizeof(struct sockaddr_storage) >= sizeof(struct sockaddr_in6), "sockaddr_storage is smaller than struct sockaddr_in6");
		(void)p;
	}
	pc_pass();
}
