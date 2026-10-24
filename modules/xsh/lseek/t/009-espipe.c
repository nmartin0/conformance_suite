/*
 * lseek() on pipes, FIFOs and sockets gives ESPIPE.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.lseek.009
 * MUTANT    lseek.espipe_as_einval
 * VARIANT   lseek.whence_values
 */
#include "pc.h"
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>

int main(void)
{
	int p[2], sv[2], r, w;

	pc_begin("XSH.lseek.009");
	pc_check(pipe(p) == 0, "pipe failed");
	pc_check(socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == 0, "socketpair failed");
	pc_check(mkfifo("q", 0600) == 0, "mkfifo failed");
	r = open("q", O_RDONLY | O_NONBLOCK);
	w = open("q", O_WRONLY);
	pc_check(r >= 0 && w >= 0, "cannot open the FIFO");
	pc_test();
	pc_expect_err(lseek(p[0], 0, SEEK_SET), ESPIPE, "lseek() on a pipe");
	pc_expect_err(lseek(w, 0, SEEK_CUR), ESPIPE, "lseek() on a FIFO");
	pc_expect_err(lseek(sv[0], 0, SEEK_SET), ESPIPE, "lseek() on a socket");
	pc_pass();
}
