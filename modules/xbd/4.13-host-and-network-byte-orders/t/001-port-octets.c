/*
 * Network byte order puts the most significant octet first: a port or an
 * address stored with htons()/htonl() has its octets in that order in
 * memory, and a socket address whose port field holds those octets reaches
 * the listener bound with htons().
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.4.13-host-and-network-byte-orders.001
 */
#include "pc.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <signal.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

static void on_alarm(int s)
{
	(void)s;
	pc_fail("the test timed out");
}

int main(void)
{
	int ls, cs, as, i;
	unsigned port = 0;
	struct sockaddr_in a, b, c;
	socklen_t len;
	unsigned char *o;
	uint16_t raw16;
	uint32_t raw32;
	unsigned char octets[2];
	struct pollfd p;

	pc_begin("XBD.4.13-host-and-network-byte-orders.001");
	(void)signal(SIGALRM, on_alarm);
	ls = socket(AF_INET, SOCK_STREAM, 0);
	if (ls < 0) {
		if (errno == EAFNOSUPPORT)
			pc_notinuse("no IPv4 sockets");
		pc_unresolved("socket failed: %s", pc_errname(errno));
	}
	memset(&a, 0, sizeof a);
	a.sin_family = AF_INET;
	a.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	for (i = 0; i < 100; i++) {
		port = 41000u + (unsigned)((getpid() * 7 + i * 131) % 20000);
		a.sin_port = htons((uint16_t)port);
		if (bind(ls, (struct sockaddr *)&a, sizeof a) == 0)
			break;
		if (errno != EADDRINUSE && errno != EACCES)
			pc_unresolved("bind failed: %s", pc_errname(errno));
	}
	if (i == 100 || listen(ls, 1) != 0)
		pc_unresolved("no port could be bound on the loopback address");
	pc_test();
	(void)alarm(30);

	/* the stored forms, byte by byte */
	raw16 = htons((uint16_t)port);
	o = (unsigned char *)&raw16;
	pc_check(o[0] == (port >> 8) && o[1] == (port & 0xff), "htons(%u) stored as %02x %02x, expected %02x %02x", port, o[0], o[1], port >> 8, port & 0xff);
	raw32 = htonl(0x7f000001u);
	o = (unsigned char *)&raw32;
	pc_check(o[0] == 127 && o[1] == 0 && o[2] == 0 && o[3] == 1, "htonl(0x7f000001) stored as %02x %02x %02x %02x", o[0], o[1], o[2], o[3]);
	pc_check(ntohs(raw16) == port, "ntohs(htons(%u)) is %u", port, (unsigned)ntohs(raw16));
	pc_check(ntohl(raw32) == 0x7f000001u, "ntohl(htonl(0x7f000001)) is %#lx", (unsigned long)ntohl(raw32));

	/* what the socket layer reports */
	len = sizeof b;
	pc_check(getsockname(ls, (struct sockaddr *)&b, &len) == 0, "getsockname failed: %s", pc_errname(errno));
	o = (unsigned char *)&b.sin_port;
	pc_check(o[0] == (port >> 8) && o[1] == (port & 0xff), "the bound port %u is reported in sin_port as %02x %02x", port, o[0], o[1]);
	o = (unsigned char *)&b.sin_addr.s_addr;
	pc_check(o[0] == 127 && o[1] == 0 && o[2] == 0 && o[3] == 1, "the bound address is reported as %u.%u.%u.%u", o[0], o[1], o[2], o[3]);

	/* a client whose port field is made from the two octets, most significant first */
	octets[0] = (unsigned char)(port >> 8);
	octets[1] = (unsigned char)(port & 0xff);
	memset(&c, 0, sizeof c);
	c.sin_family = AF_INET;
	memcpy(&c.sin_port, octets, 2);
	{
		unsigned char ab[4] = { 127, 0, 0, 1 };

		memcpy(&c.sin_addr.s_addr, ab, 4);
	}
	cs = socket(AF_INET, SOCK_STREAM, 0);
	if (cs < 0)
		pc_unresolved("socket failed: %s", pc_errname(errno));
	pc_check(connect(cs, (struct sockaddr *)&c, sizeof c) == 0, "connect to an address built from the octets %02x %02x / 127.0.0.1 failed: %s", octets[0], octets[1], pc_errname(errno));
	p.fd = ls;
	p.events = POLLIN;
	p.revents = 0;
	pc_check(poll(&p, 1, 5000) == 1, "the listener did not see the connection");
	as = accept(ls, NULL, NULL);
	pc_check(as >= 0, "accept failed: %s", pc_errname(errno));
	(void)close(as);
	(void)close(cs);
	(void)close(ls);
	pc_pass();
}
