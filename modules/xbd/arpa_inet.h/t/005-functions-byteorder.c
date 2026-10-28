/*
 * <arpa/inet.h> functions byteorder
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.arpa_inet.h.005
 *
 * Only <arpa/inet.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <arpa/inet.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.arpa_inet.h.005");
	pc_test();
#ifndef htonl
	if (!IS_T(&(htonl), uint32_t (*)(uint32_t)))
		pc_fail("htonl() is not declared as uint32_t htonl(uint32_t)");
	if (IS_T(&(htonl), uint32_t (*)(uint32_t, int)))
		pc_fail("htonl() is declared without a prototype");
#endif
#ifndef htons
	if (!IS_T(&(htons), uint16_t (*)(uint16_t)))
		pc_fail("htons() is not declared as uint16_t htons(uint16_t)");
	if (IS_T(&(htons), uint16_t (*)(uint16_t, int)))
		pc_fail("htons() is declared without a prototype");
#endif
#ifndef ntohl
	if (!IS_T(&(ntohl), uint32_t (*)(uint32_t)))
		pc_fail("ntohl() is not declared as uint32_t ntohl(uint32_t)");
	if (IS_T(&(ntohl), uint32_t (*)(uint32_t, int)))
		pc_fail("ntohl() is declared without a prototype");
#endif
#ifndef ntohs
	if (!IS_T(&(ntohs), uint16_t (*)(uint16_t)))
		pc_fail("ntohs() is not declared as uint16_t ntohs(uint16_t)");
	if (IS_T(&(ntohs), uint16_t (*)(uint16_t, int)))
		pc_fail("ntohs() is declared without a prototype");
#endif
	pc_pass();
}
