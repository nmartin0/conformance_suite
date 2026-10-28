/*
 * Socket functions listed as cancellation points act on a pending
 * cancellation request before they return.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.013
 *
 * For each of accept(), accept4(), connect(), recv(), recvfrom(),
 * recvmsg(), send(), sendto() and sendmsg(): see XSH.2.9-threads.012.
 */
#include "pccp.h"
#include <fcntl.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>

static volatile long sink;
static int lfd;
static int sv[2];
static int cfd;
static int extra;
static struct sockaddr_un addr;
static socklen_t alen;

static void s_client(void)
{
	extra = socket(AF_UNIX, SOCK_STREAM, 0);
	if (extra != -1)
		sink = connect(extra, (struct sockaddr *)&addr, alen);
}
static void s_newsock(void) { cfd = socket(AF_UNIX, SOCK_STREAM, 0); }
static void s_data(void) { sink = send(sv[1], "x", 1, MSG_NOSIGNAL); }
static void d_extra(void) { if (extra != -1) (void)close(extra); extra = -1; }

static void c_accept(void) { sink = accept(lfd, NULL, NULL); }
static void c_accept4(void) { sink = accept4(lfd, NULL, NULL, 0); }
static void c_connect(void) { sink = connect(cfd, (struct sockaddr *)&addr, alen); }
static void c_recv(void) { char b; sink = recv(sv[0], &b, 1, 0); }
static void c_recvfrom(void) { char b; sink = recvfrom(sv[0], &b, 1, 0, NULL, NULL); }
static void c_recvmsg(void)
{
	char b;
	struct iovec v;
	struct msghdr m;

	v.iov_base = &b; v.iov_len = 1;
	memset(&m, 0, sizeof m);
	m.msg_iov = &v; m.msg_iovlen = 1;
	sink = recvmsg(sv[0], &m, 0);
}
static void c_send(void) { sink = send(sv[1], "y", 1, MSG_NOSIGNAL); }
static void c_sendto(void) { sink = sendto(sv[1], "y", 1, MSG_NOSIGNAL, NULL, 0); }
static void c_sendmsg(void)
{
	struct iovec v;
	struct msghdr m;

	v.iov_base = (void *)"y"; v.iov_len = 1;
	memset(&m, 0, sizeof m);
	m.msg_iov = &v; m.msg_iovlen = 1;
	sink = sendmsg(sv[1], &m, MSG_NOSIGNAL);
}

static const struct cpcase cases[] = {
	{ "accept", s_client, c_accept, d_extra },
	{ "accept4", s_client, c_accept4, d_extra },
	{ "connect", s_newsock, c_connect, NULL },
	{ "recv", s_data, c_recv, NULL },
	{ "recvfrom", s_data, c_recvfrom, NULL },
	{ "recvmsg", s_data, c_recvmsg, NULL },
	{ "send", NULL, c_send, NULL },
	{ "sendto", NULL, c_sendto, NULL },
	{ "sendmsg", NULL, c_sendmsg, NULL },
};

int main(void)
{
	char bad[512];
	int n;

	pc_begin("XSH.2.9-threads.013");
	guard(250);
	(void)signal(SIGPIPE, SIG_IGN);
	memset(&addr, 0, sizeof addr);
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, "sock");
	alen = (socklen_t)(offsetof(struct sockaddr_un, sun_path) + strlen("sock") + 1);
	lfd = socket(AF_UNIX, SOCK_STREAM, 0);
	NEED(lfd != -1, "socket failed");
	(void)unlink("sock");
	NEED(bind(lfd, (struct sockaddr *)&addr, alen) == 0, "bind failed");
	NEED(listen(lfd, 16) == 0, "listen failed");
	NEED(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	pc_test();
	n = cp_run(cases, (int)(sizeof cases / sizeof cases[0]), CP_PENDING, bad, sizeof bad);
	(void)unlink("sock");
	pc_check(n == 0, "with a cancellation request pending and cancelability enabled the thread was not canceled in: %s", bad);
	pc_pass();
}
