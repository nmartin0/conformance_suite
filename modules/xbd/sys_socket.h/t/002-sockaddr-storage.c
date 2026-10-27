/*
 * sockaddr_storage is large and aligned enough for sockaddr_in and
 *   sockaddr_un, with ss_family where the families are.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.002
 */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>
#include <stddef.h>
#include "pc.h"

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	struct sockaddr_storage ss;

	pc_begin("XBD.sys_socket.h.002");
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
		struct sockaddr_in p;

		pc_check(IS_T(p.sin_family, sa_family_t), "sin_family does not have type sa_family_t");
		pc_check(offsetof(struct sockaddr_storage, ss_family) == offsetof(struct sockaddr_in, sin_family), "ss_family and sin_family are at different offsets");
		pc_check(_Alignof(struct sockaddr_storage) % _Alignof(struct sockaddr_in) == 0, "sockaddr_storage is aligned less strictly than struct sockaddr_in");
		pc_check(sizeof(struct sockaddr_storage) >= sizeof(struct sockaddr_in), "sockaddr_storage is smaller than struct sockaddr_in");
		(void)p;
	}
	{
		struct sockaddr_un p;

		pc_check(IS_T(p.sun_family, sa_family_t), "sun_family does not have type sa_family_t");
		pc_check(offsetof(struct sockaddr_storage, ss_family) == offsetof(struct sockaddr_un, sun_family), "ss_family and sun_family are at different offsets");
		pc_check(_Alignof(struct sockaddr_storage) % _Alignof(struct sockaddr_un) == 0, "sockaddr_storage is aligned less strictly than struct sockaddr_un");
		pc_check(sizeof(struct sockaddr_storage) >= sizeof(struct sockaddr_un), "sockaddr_storage is smaller than struct sockaddr_un");
		(void)p;
	}
	pc_pass();
}
