/*
 * A receive on a stream socket never returns data from more than one
 * segment, and ancillary data comes with the first byte of its segment.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.005
 *
 * On an AF_UNIX stream pair three bytes are sent without ancillary data
 * and then three bytes with an SCM_RIGHTS control message; the second
 * group starts a segment of its own. A receive for six bytes returns no
 * more than the first three, with no control message; once those are
 * consumed, a receive of one byte returns the first byte of the second
 * group together with the descriptor.
 */
#include "pcsk.h"

union cbuf {
	char b[CMSG_SPACE(sizeof(int))];
	struct cmsghdr align;
};

static ssize_t recv_msg(int fd, void *buf, size_t n, union cbuf *c, struct msghdr *m, struct iovec *v)
{
	v->iov_base = buf;
	v->iov_len = n;
	memset(m, 0, sizeof *m);
	memset(c, 0, sizeof *c);
	m->msg_iov = v;
	m->msg_iovlen = 1;
	m->msg_control = c->b;
	m->msg_controllen = sizeof c->b;
	return recvmsg(fd, m, 0);
}

int main(void)
{
	int sv[2], file, got = 0, fdnew = -1;
	char buf[16];
	union cbuf cs, cr;
	struct msghdr m;
	struct iovec v;
	struct cmsghdr *h;
	ssize_t r;

	pc_begin("XSH.2.10-sockets.005");
	guard(60);
	pc_make_file("f", "z");
	file = open("f", O_RDONLY);
	NEED(file != -1, "open failed");
	NEED(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed: %s", pc_errname(errno));
	NEED(send(sv[0], "abc", 3, MSG_NOSIGNAL) == 3, "send failed: %s", pc_errname(errno));
	memset(&m, 0, sizeof m);
	memset(&cs, 0, sizeof cs);
	v.iov_base = (void *)"def";
	v.iov_len = 3;
	m.msg_iov = &v;
	m.msg_iovlen = 1;
	m.msg_control = cs.b;
	m.msg_controllen = sizeof cs.b;
	h = CMSG_FIRSTHDR(&m);
	h->cmsg_level = SOL_SOCKET;
	h->cmsg_type = SCM_RIGHTS;
	h->cmsg_len = CMSG_LEN(sizeof(int));
	memcpy(CMSG_DATA(h), &file, sizeof(int));
	NEED(sendmsg(sv[0], &m, MSG_NOSIGNAL) == 3, "sendmsg with SCM_RIGHTS failed: %s", pc_errname(errno));
	pc_test();

	NEED(wait_fd(sv[1], POLLIN, 5000) > 0, "nothing to receive");
	nap_ms(100);
	r = recv_msg(sv[1], buf, 6, &cr, &m, &v);
	pc_check(r > 0 && r <= 3, "a receive for six bytes returned %ld bytes while the first segment has three", (long)r);
	pc_check(m.msg_controllen == 0 && CMSG_FIRSTHDR(&m) == NULL, "the first segment carried a control message");
	pc_check(memcmp(buf, "abc", (size_t)r) == 0, "the bytes received are not the start of the first group");
	got = (int)r;
	while (got < 3) {
		NEED(wait_fd(sv[1], POLLIN, 5000) > 0, "the rest of the first group did not arrive");
		r = recv_msg(sv[1], buf + got, (size_t)(3 - got), &cr, &m, &v);
		pc_check(r > 0, "receive returned %ld", (long)r);
		pc_check(CMSG_FIRSTHDR(&m) == NULL, "the first segment carried a control message");
		got += (int)r;
	}
	r = recv_msg(sv[1], buf + 3, 1, &cr, &m, &v);
	pc_check(r == 1 && buf[3] == 'd', "a receive of one byte after the first group returned %ld bytes, expected the byte 'd'", (long)r);
	h = CMSG_FIRSTHDR(&m);
	pc_check(h != NULL && h->cmsg_level == SOL_SOCKET && h->cmsg_type == SCM_RIGHTS && h->cmsg_len >= CMSG_LEN(sizeof(int)),
	    "the first byte of the second group did not come with its SCM_RIGHTS control message");
	memcpy(&fdnew, CMSG_DATA(h), sizeof(int));
	pc_check(fdnew >= 0 && fcntl(fdnew, F_GETFL) != -1, "the descriptor received is not open");
	pc_pass();
}
