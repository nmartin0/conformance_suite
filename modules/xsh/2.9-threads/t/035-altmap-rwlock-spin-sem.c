/*
 * A process-shared read-write lock, spin lock and semaphore work through
 * another mapping of the memory they are in.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.2.9-threads.035
 *
 * A file is mapped twice (two addresses); each object is initialized
 * through the first mapping and used through both. Whether another thread
 * can get a lock is found by a trylock made in a thread of its own.
 */
#include "pcth.h"
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>

static unsigned char *m1, *m2;

#define OFF_RW 0
#define OFF_SPIN 256
#define OFF_SEM 512

static const char *rn(int r)
{
	return r == 0 ? "0" : pc_errname(r);
}

struct probe {
	int kind;	/* 0 tryrdlock, 1 trywrlock, 2 spin trylock */
	unsigned char *base;
};

static void *probe_main(void *a)
{
	struct probe *p = a;
	int r;

	if (p->kind == 0) {
		r = pthread_rwlock_tryrdlock((pthread_rwlock_t *)(void *)(p->base + OFF_RW));
		if (r == 0)
			(void)pthread_rwlock_unlock((pthread_rwlock_t *)(void *)(p->base + OFF_RW));
	} else if (p->kind == 1) {
		r = pthread_rwlock_trywrlock((pthread_rwlock_t *)(void *)(p->base + OFF_RW));
		if (r == 0)
			(void)pthread_rwlock_unlock((pthread_rwlock_t *)(void *)(p->base + OFF_RW));
	} else {
		r = pthread_spin_trylock((pthread_spinlock_t *)(void *)(p->base + OFF_SPIN));
		if (r == 0)
			(void)pthread_spin_unlock((pthread_spinlock_t *)(void *)(p->base + OFF_SPIN));
	}
	return (void *)(long)r;
}

static int probe(int kind, unsigned char *base)
{
	struct probe p;
	pthread_t t;
	void *st;

	p.kind = kind;
	p.base = base;
	t = start(probe_main, &p);
	NEED(join_within(t, 10000, &st), "a probe thread did not end");
	return (int)(long)st;
}

int main(void)
{
	pthread_rwlockattr_t ra;
	pthread_rwlock_t *rw;
	pthread_spinlock_t *sp;
	sem_t *sm;
	int fd, r;
	long ps = sysconf(_SC_PAGESIZE);

	pc_begin("XSH.2.9-threads.035");
	guard(120);
	fd = open("shm", O_RDWR | O_CREAT | O_EXCL, 0600);
	NEED(fd != -1, "open failed");
	NEED(ftruncate(fd, ps) == 0, "ftruncate failed");
	m1 = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	m2 = mmap(NULL, (size_t)ps, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	NEED(m1 != MAP_FAILED && m2 != MAP_FAILED, "mmap failed");
	NEED(m1 != m2, "the two mappings have the same address");
	rw = (pthread_rwlock_t *)(void *)(m1 + OFF_RW);
	sp = (pthread_spinlock_t *)(void *)(m1 + OFF_SPIN);
	sm = (sem_t *)(void *)(m1 + OFF_SEM);
	NEED(pthread_rwlockattr_init(&ra) == 0, "pthread_rwlockattr_init failed");
	NEED(pthread_rwlockattr_setpshared(&ra, PTHREAD_PROCESS_SHARED) == 0, "pthread_rwlockattr_setpshared failed");
	r = pthread_rwlock_init(rw, &ra);
	NEED(r == 0, "pthread_rwlock_init failed: %s", pc_errname(r));
	r = pthread_spin_init(sp, PTHREAD_PROCESS_SHARED);
	NEED(r == 0, "pthread_spin_init failed: %s", pc_errname(r));
	NEED(sem_init(sm, 1, 0) == 0, "sem_init failed: %s", pc_errname(errno));
	pc_test();

	/* read-write lock: write-locked through the second mapping, probed through the first */
	r = pthread_rwlock_wrlock((pthread_rwlock_t *)(void *)(m2 + OFF_RW));
	NEED(r == 0, "pthread_rwlock_wrlock through the second mapping failed: %s", rn(r));
	r = probe(0, m1);
	pc_check(r == EBUSY, "rwlock: tryrdlock through the first mapping of a lock write-locked through the second returned %s, expected EBUSY", rn(r));
	r = pthread_rwlock_unlock(rw);
	pc_check(r == 0, "rwlock: pthread_rwlock_unlock through the first mapping of a lock locked through the second returned %s", rn(r));
	r = probe(1, m2);
	pc_check(r == 0, "rwlock: trywrlock through the second mapping after the unlock through the first returned %s, expected 0", rn(r));
	r = pthread_rwlock_rdlock(rw);
	NEED(r == 0, "pthread_rwlock_rdlock failed: %s", rn(r));
	r = probe(1, m2);
	pc_check(r == EBUSY, "rwlock: trywrlock through the second mapping of a lock read-locked through the first returned %s, expected EBUSY", rn(r));
	r = pthread_rwlock_unlock((pthread_rwlock_t *)(void *)(m2 + OFF_RW));
	pc_check(r == 0, "rwlock: unlock through the second mapping returned %s", rn(r));

	/* spin lock */
	r = pthread_spin_lock((pthread_spinlock_t *)(void *)(m2 + OFF_SPIN));
	NEED(r == 0, "pthread_spin_lock through the second mapping failed: %s", rn(r));
	r = probe(2, m1);
	pc_check(r == EBUSY, "spin lock: trylock through the first mapping of a lock held through the second returned %s, expected EBUSY", rn(r));
	r = pthread_spin_unlock(sp);
	pc_check(r == 0, "spin lock: unlock through the first mapping returned %s", rn(r));
	r = probe(2, m2);
	pc_check(r == 0, "spin lock: trylock through the second mapping after the unlock returned %s, expected 0", rn(r));

	/* semaphore */
	pc_check(sem_post((sem_t *)(void *)(m2 + OFF_SEM)) == 0, "semaphore: sem_post through the second mapping failed");
	pc_check(sem_trywait(sm) == 0, "semaphore: sem_trywait through the first mapping did not take the post made through the second");
	pc_check(sem_trywait(sm) == -1 && errno == EAGAIN, "semaphore: a second sem_trywait did not fail with EAGAIN");
	pc_pass();
}
