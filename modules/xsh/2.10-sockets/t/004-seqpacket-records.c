/*
 * A SOCK_SEQPACKET socket keeps record boundaries: one receive never
 * returns parts of two records.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.10-sockets.004
 *
 * A socketpair() of type SOCK_SEQPACKET (AF_UNIX); the test is not in use
 * if the system makes none. Records of 3, 5 and 2 bytes sent before any is
 * received come out one per receive, whatever the size of the buffer; the
 * pair is full-duplex. The record is always read whole (buffer larger than
 * the record), since what a short buffer does is not settled by 2.10.6.
 */
#include "pcsk.h"

int main(void)
{
	int sv[2], r;
	char buf[100];

	pc_begin("XSH.2.10-sockets.004");
	guard(60);
	r = socketpair(AF_UNIX, SOCK_SEQPACKET, 0, sv);
	if (r == -1 && (errno == EPROTOTYPE || errno == EOPNOTSUPP || errno == EPROTONOSUPPORT || errno == EAFNOSUPPORT
#ifdef ESOCKTNOSUPPORT
	    || errno == ESOCKTNOSUPPORT
#endif
	    ))
		pc_notinuse("socketpair(AF_UNIX, SOCK_SEQPACKET) is not supported (%s)", pc_errname(errno));
	NEED(r == 0, "socketpair failed: %s", pc_errname(errno));
	NEED(send(sv[0], "abc", 3, MSG_NOSIGNAL) == 3, "send of the first record failed: %s", pc_errname(errno));
	NEED(send(sv[0], "defgh", 5, MSG_NOSIGNAL) == 5, "send of the second record failed: %s", pc_errname(errno));
	NEED(send(sv[0], "ij", 2, MSG_NOSIGNAL) == 2, "send of the third record failed: %s", pc_errname(errno));
	pc_test();

	NEED(wait_fd(sv[1], POLLIN, 5000) > 0, "nothing to receive");
	r = (int)recv(sv[1], buf, sizeof buf, 0);
	pc_check(r == 3 && memcmp(buf, "abc", 3) == 0, "the first receive returned %d bytes, expected the 3 byte record alone", r);
	r = (int)recv(sv[1], buf, sizeof buf, 0);
	pc_check(r == 5 && memcmp(buf, "defgh", 5) == 0, "the second receive returned %d bytes, expected the 5 byte record alone", r);
	r = (int)recv(sv[1], buf, sizeof buf, 0);
	pc_check(r == 2 && memcmp(buf, "ij", 2) == 0, "the third receive returned %d bytes, expected the 2 byte record alone", r);

	NEED(send(sv[1], "klmn", 4, MSG_NOSIGNAL) == 4, "send in the other direction failed: %s", pc_errname(errno));
	NEED(send(sv[1], "o", 1, MSG_NOSIGNAL) == 1, "send in the other direction failed: %s", pc_errname(errno));
	NEED(wait_fd(sv[0], POLLIN, 5000) > 0, "nothing to receive");
	r = (int)recv(sv[0], buf, sizeof buf, 0);
	pc_check(r == 4 && memcmp(buf, "klmn", 4) == 0, "in the other direction the first receive returned %d bytes, expected the 4 byte record alone", r);
	r = (int)recv(sv[0], buf, sizeof buf, 0);
	pc_check(r == 1 && buf[0] == 'o', "in the other direction the second receive returned %d bytes, expected the 1 byte record alone", r);
	pc_pass();
}
