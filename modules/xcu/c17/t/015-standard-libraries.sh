# -l pthread, rt and xnet
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.015
# -l pthread, -l rt and -l xnet make the thread, semaphore and socket
# interfaces available and the programs run.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >t1.c <<'EOY'
#include <pthread.h>
#include <stdio.h>
static void *run(void *a) { *(int *)a = 42; return a; }
int main(void)
{
	pthread_t t;
	int v = 0;

	if (pthread_create(&t, 0, run, &v) != 0)
		return 1;
	if (pthread_join(t, 0) != 0)
		return 2;
	return v == 42 ? 0 : 3;
}
EOY
cat >t2.c <<'EOY'
#include <semaphore.h>
#include <aio.h>
#include <sched.h>
int main(void)
{
	sem_t s;

	if (sem_init(&s, 0, 1) != 0)
		return 1;
	if (sem_wait(&s) != 0)
		return 2;
	if (sem_post(&s) != 0)
		return 3;
	sem_destroy(&s);
	return sched_yield() == 0 ? 0 : 4;
}
EOY
cat >t3.c <<'EOY'
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
int main(void)
{
	struct in_addr a;
	int fds[2];

	if (inet_pton(AF_INET, "127.0.0.1", &a) != 1)
		return 1;
	if (ntohl(a.s_addr) != 0x7f000001UL)
		return 2;
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) != 0)
		return 3;
	close(fds[0]);
	close(fds[1]);
	return 0;
}
EOY
cat >t4.c <<'EOY'
#include <math.h>
int main(void) { volatile double x = 16.0; return sqrt(x) == 4.0 ? 0 : 1; }
EOY
pc_test
for n in 1:pthread 2:rt 3:xnet 4:m; do
	i=${n%%:*}; l=${n#*:}
	c17 -o p$i t$i.c -l $l </dev/null >out 2>err || pc_fail "linking with -l $l failed"
	./p$i </dev/null >out 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "the program built with -l $l exits $rc"
done
c17 -o p5 t1.c -l pthread -l rt -l xnet -l m </dev/null >out 2>err || pc_fail "several -l options together failed"
./p5 </dev/null
[ "$?" -eq 0 ] || pc_fail "the program built with several -l options failed"
pc_pass
