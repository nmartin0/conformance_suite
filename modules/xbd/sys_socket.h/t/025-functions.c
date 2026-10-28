/*
 * <sys/socket.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.sys_socket.h.025
 *
 * Only <sys/socket.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <sys/socket.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.sys_socket.h.025");
	pc_test();
	if (!IS_T(&(accept), int (*)(int, struct sockaddr *restrict, socklen_t *restrict)))
		pc_fail("accept() is not declared as int accept(int, struct sockaddr *restrict, socklen_t *restrict)");
	if (IS_T(&(accept), int (*)(int, struct sockaddr *restrict, socklen_t *restrict, int)))
		pc_fail("accept() is declared without a prototype");
	if (!IS_T(&(bind), int (*)(int, const struct sockaddr *, socklen_t)))
		pc_fail("bind() is not declared as int bind(int, const struct sockaddr *, socklen_t)");
	if (IS_T(&(bind), int (*)(int, const struct sockaddr *, socklen_t, int)))
		pc_fail("bind() is declared without a prototype");
	if (!IS_T(&(connect), int (*)(int, const struct sockaddr *, socklen_t)))
		pc_fail("connect() is not declared as int connect(int, const struct sockaddr *, socklen_t)");
	if (IS_T(&(connect), int (*)(int, const struct sockaddr *, socklen_t, int)))
		pc_fail("connect() is declared without a prototype");
	if (!IS_T(&(getpeername), int (*)(int, struct sockaddr *restrict, socklen_t *restrict)))
		pc_fail("getpeername() is not declared as int getpeername(int, struct sockaddr *restrict, socklen_t *restrict)");
	if (IS_T(&(getpeername), int (*)(int, struct sockaddr *restrict, socklen_t *restrict, int)))
		pc_fail("getpeername() is declared without a prototype");
	if (!IS_T(&(getsockname), int (*)(int, struct sockaddr *restrict, socklen_t *restrict)))
		pc_fail("getsockname() is not declared as int getsockname(int, struct sockaddr *restrict, socklen_t *restrict)");
	if (IS_T(&(getsockname), int (*)(int, struct sockaddr *restrict, socklen_t *restrict, int)))
		pc_fail("getsockname() is declared without a prototype");
	if (!IS_T(&(getsockopt), int (*)(int, int, int, void *restrict, socklen_t *restrict)))
		pc_fail("getsockopt() is not declared as int getsockopt(int, int, int, void *restrict, socklen_t *restrict)");
	if (IS_T(&(getsockopt), int (*)(int, int, int, void *restrict, socklen_t *restrict, int)))
		pc_fail("getsockopt() is declared without a prototype");
	if (!IS_T(&(listen), int (*)(int, int)))
		pc_fail("listen() is not declared as int listen(int, int)");
	if (IS_T(&(listen), int (*)(int, int, int)))
		pc_fail("listen() is declared without a prototype");
	if (!IS_T(&(recv), ssize_t (*)(int, void *, size_t, int)))
		pc_fail("recv() is not declared as ssize_t recv(int, void *, size_t, int)");
	if (IS_T(&(recv), ssize_t (*)(int, void *, size_t, int, int)))
		pc_fail("recv() is declared without a prototype");
	if (!IS_T(&(recvfrom), ssize_t (*)(int, void *restrict, size_t, int, struct sockaddr *restrict, socklen_t *restrict)))
		pc_fail("recvfrom() is not declared as ssize_t recvfrom(int, void *restrict, size_t, int, struct sockaddr *restrict, socklen_t *restrict)");
	if (IS_T(&(recvfrom), ssize_t (*)(int, void *restrict, size_t, int, struct sockaddr *restrict, socklen_t *restrict, int)))
		pc_fail("recvfrom() is declared without a prototype");
	if (!IS_T(&(recvmsg), ssize_t (*)(int, struct msghdr *, int)))
		pc_fail("recvmsg() is not declared as ssize_t recvmsg(int, struct msghdr *, int)");
	if (IS_T(&(recvmsg), ssize_t (*)(int, struct msghdr *, int, int)))
		pc_fail("recvmsg() is declared without a prototype");
	if (!IS_T(&(send), ssize_t (*)(int, const void *, size_t, int)))
		pc_fail("send() is not declared as ssize_t send(int, const void *, size_t, int)");
	if (IS_T(&(send), ssize_t (*)(int, const void *, size_t, int, int)))
		pc_fail("send() is declared without a prototype");
	if (!IS_T(&(sendmsg), ssize_t (*)(int, const struct msghdr *, int)))
		pc_fail("sendmsg() is not declared as ssize_t sendmsg(int, const struct msghdr *, int)");
	if (IS_T(&(sendmsg), ssize_t (*)(int, const struct msghdr *, int, int)))
		pc_fail("sendmsg() is declared without a prototype");
	if (!IS_T(&(sendto), ssize_t (*)(int, const void *, size_t, int, const struct sockaddr *, socklen_t)))
		pc_fail("sendto() is not declared as ssize_t sendto(int, const void *, size_t, int, const struct sockaddr *, socklen_t)");
	if (IS_T(&(sendto), ssize_t (*)(int, const void *, size_t, int, const struct sockaddr *, socklen_t, int)))
		pc_fail("sendto() is declared without a prototype");
	if (!IS_T(&(setsockopt), int (*)(int, int, int, const void *, socklen_t)))
		pc_fail("setsockopt() is not declared as int setsockopt(int, int, int, const void *, socklen_t)");
	if (IS_T(&(setsockopt), int (*)(int, int, int, const void *, socklen_t, int)))
		pc_fail("setsockopt() is declared without a prototype");
	if (!IS_T(&(shutdown), int (*)(int, int)))
		pc_fail("shutdown() is not declared as int shutdown(int, int)");
	if (IS_T(&(shutdown), int (*)(int, int, int)))
		pc_fail("shutdown() is declared without a prototype");
	if (!IS_T(&(sockatmark), int (*)(int)))
		pc_fail("sockatmark() is not declared as int sockatmark(int)");
	if (IS_T(&(sockatmark), int (*)(int, int)))
		pc_fail("sockatmark() is declared without a prototype");
	if (!IS_T(&(socket), int (*)(int, int, int)))
		pc_fail("socket() is not declared as int socket(int, int, int)");
	if (IS_T(&(socket), int (*)(int, int, int, int)))
		pc_fail("socket() is declared without a prototype");
	if (!IS_T(&(socketpair), int (*)(int, int, int, int [2])))
		pc_fail("socketpair() is not declared as int socketpair(int, int, int, int [2])");
	if (IS_T(&(socketpair), int (*)(int, int, int, int [2], int)))
		pc_fail("socketpair() is declared without a prototype");
	pc_pass();
}
