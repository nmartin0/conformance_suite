/*
 * <netdb.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.netdb.h.012
 *
 * Only <netdb.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <netdb.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.netdb.h.012");
	pc_test();
	if (!IS_T(&(endhostent), void (*)(void)))
		pc_fail("endhostent() is not declared as void endhostent(void)");
	if (IS_T(&(endhostent), void (*)(int)))
		pc_fail("endhostent() is declared without a prototype");
	if (!IS_T(&(endnetent), void (*)(void)))
		pc_fail("endnetent() is not declared as void endnetent(void)");
	if (IS_T(&(endnetent), void (*)(int)))
		pc_fail("endnetent() is declared without a prototype");
	if (!IS_T(&(endprotoent), void (*)(void)))
		pc_fail("endprotoent() is not declared as void endprotoent(void)");
	if (IS_T(&(endprotoent), void (*)(int)))
		pc_fail("endprotoent() is declared without a prototype");
	if (!IS_T(&(endservent), void (*)(void)))
		pc_fail("endservent() is not declared as void endservent(void)");
	if (IS_T(&(endservent), void (*)(int)))
		pc_fail("endservent() is declared without a prototype");
	if (!IS_T(&(freeaddrinfo), void (*)(struct addrinfo *)))
		pc_fail("freeaddrinfo() is not declared as void freeaddrinfo(struct addrinfo *)");
	if (IS_T(&(freeaddrinfo), void (*)(struct addrinfo *, int)))
		pc_fail("freeaddrinfo() is declared without a prototype");
	if (!IS_T(&(gai_strerror), const char *(*)(int)))
		pc_fail("gai_strerror() is not declared as const char *gai_strerror(int)");
	if (IS_T(&(gai_strerror), const char *(*)(int, int)))
		pc_fail("gai_strerror() is declared without a prototype");
	if (!IS_T(&(getaddrinfo), int (*)(const char *restrict, const char *restrict, const struct addrinfo *restrict, struct addrinfo **restrict)))
		pc_fail("getaddrinfo() is not declared as int getaddrinfo(const char *restrict, const char *restrict, const struct addrinfo *restrict, struct addrinfo **restrict)");
	if (IS_T(&(getaddrinfo), int (*)(const char *restrict, const char *restrict, const struct addrinfo *restrict, struct addrinfo **restrict, int)))
		pc_fail("getaddrinfo() is declared without a prototype");
	if (!IS_T(&(gethostent), struct hostent *(*)(void)))
		pc_fail("gethostent() is not declared as struct hostent *gethostent(void)");
	if (IS_T(&(gethostent), struct hostent *(*)(int)))
		pc_fail("gethostent() is declared without a prototype");
	if (!IS_T(&(getnameinfo), int (*)(const struct sockaddr *restrict, socklen_t, char *restrict, socklen_t, char *restrict, socklen_t, int)))
		pc_fail("getnameinfo() is not declared as int getnameinfo(const struct sockaddr *restrict, socklen_t, char *restrict, socklen_t, char *restrict, socklen_t, int)");
	if (IS_T(&(getnameinfo), int (*)(const struct sockaddr *restrict, socklen_t, char *restrict, socklen_t, char *restrict, socklen_t, int, int)))
		pc_fail("getnameinfo() is declared without a prototype");
	if (!IS_T(&(getnetbyaddr), struct netent *(*)(uint32_t, int)))
		pc_fail("getnetbyaddr() is not declared as struct netent *getnetbyaddr(uint32_t, int)");
	if (IS_T(&(getnetbyaddr), struct netent *(*)(uint32_t, int, int)))
		pc_fail("getnetbyaddr() is declared without a prototype");
	if (!IS_T(&(getnetbyname), struct netent *(*)(const char *)))
		pc_fail("getnetbyname() is not declared as struct netent *getnetbyname(const char *)");
	if (IS_T(&(getnetbyname), struct netent *(*)(const char *, int)))
		pc_fail("getnetbyname() is declared without a prototype");
	if (!IS_T(&(getnetent), struct netent *(*)(void)))
		pc_fail("getnetent() is not declared as struct netent *getnetent(void)");
	if (IS_T(&(getnetent), struct netent *(*)(int)))
		pc_fail("getnetent() is declared without a prototype");
	if (!IS_T(&(getprotobyname), struct protoent *(*)(const char *)))
		pc_fail("getprotobyname() is not declared as struct protoent *getprotobyname(const char *)");
	if (IS_T(&(getprotobyname), struct protoent *(*)(const char *, int)))
		pc_fail("getprotobyname() is declared without a prototype");
	if (!IS_T(&(getprotobynumber), struct protoent *(*)(int)))
		pc_fail("getprotobynumber() is not declared as struct protoent *getprotobynumber(int)");
	if (IS_T(&(getprotobynumber), struct protoent *(*)(int, int)))
		pc_fail("getprotobynumber() is declared without a prototype");
	if (!IS_T(&(getprotoent), struct protoent *(*)(void)))
		pc_fail("getprotoent() is not declared as struct protoent *getprotoent(void)");
	if (IS_T(&(getprotoent), struct protoent *(*)(int)))
		pc_fail("getprotoent() is declared without a prototype");
	if (!IS_T(&(getservbyname), struct servent *(*)(const char *, const char *)))
		pc_fail("getservbyname() is not declared as struct servent *getservbyname(const char *, const char *)");
	if (IS_T(&(getservbyname), struct servent *(*)(const char *, const char *, int)))
		pc_fail("getservbyname() is declared without a prototype");
	if (!IS_T(&(getservbyport), struct servent *(*)(int, const char *)))
		pc_fail("getservbyport() is not declared as struct servent *getservbyport(int, const char *)");
	if (IS_T(&(getservbyport), struct servent *(*)(int, const char *, int)))
		pc_fail("getservbyport() is declared without a prototype");
	if (!IS_T(&(getservent), struct servent *(*)(void)))
		pc_fail("getservent() is not declared as struct servent *getservent(void)");
	if (IS_T(&(getservent), struct servent *(*)(int)))
		pc_fail("getservent() is declared without a prototype");
	if (!IS_T(&(sethostent), void (*)(int)))
		pc_fail("sethostent() is not declared as void sethostent(int)");
	if (IS_T(&(sethostent), void (*)(int, int)))
		pc_fail("sethostent() is declared without a prototype");
	if (!IS_T(&(setnetent), void (*)(int)))
		pc_fail("setnetent() is not declared as void setnetent(int)");
	if (IS_T(&(setnetent), void (*)(int, int)))
		pc_fail("setnetent() is declared without a prototype");
	if (!IS_T(&(setprotoent), void (*)(int)))
		pc_fail("setprotoent() is not declared as void setprotoent(int)");
	if (IS_T(&(setprotoent), void (*)(int, int)))
		pc_fail("setprotoent() is declared without a prototype");
	if (!IS_T(&(setservent), void (*)(int)))
		pc_fail("setservent() is not declared as void setservent(int)");
	if (IS_T(&(setservent), void (*)(int, int)))
		pc_fail("setservent() is declared without a prototype");
	pc_pass();
}
