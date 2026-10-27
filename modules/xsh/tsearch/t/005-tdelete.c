/*
 * tdelete() removes nodes.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.tsearch.005
 */
#include "pc.h"
#include <search.h>
#include <stdlib.h>
#include <string.h>

#define MAXEV 4096

struct ev {
	const void *node;
	VISIT v;
	int level;
};

static struct ev evs[MAXEV];
static int nev, evover;

static void act(const void *node, VISIT v, int level)
{
	if (nev < MAXEV) {
		evs[nev].node = node;
		evs[nev].v = v;
		evs[nev].level = level;
		nev++;
	} else
		evover = 1;
}

/* The elements are pointers to int; the order is the numeric one. */
static int cmp(const void *a, const void *b)
{
	int x = *(const int *)a, y = *(const int *)b;

	return x < y ? -1 : x > y;
}

/* The element stored in a node: the int the stored pointer refers to. */
static int elem(const void *node)
{
	return **(int *const *)node;
}

/*
 * Walk the tree below root and check that the events describe a tree:
 * levels follow the nesting, a node that has children is visited three times
 * in order, a childless one once as a leaf, and the elements come out in
 * ascending order. Fills keys[] with the elements in order; returns their
 * number.
 */
static int walk_check(const void *root, int *keys, int maxkeys)
{
	const void *stk[MAXEV];
	int seen[MAXEV];
	int depth = 0, nk = 0, i;

	nev = 0;
	evover = 0;
	twalk(root, act);
	pc_check(!evover, "twalk() made more than %d calls", MAXEV);
	pc_check(nev > 0, "twalk() made no calls");
	for (i = 0; i < nev; i++) {
		const struct ev *e = &evs[i];
		int emit = 0;

		switch (e->v) {
		case preorder:
			pc_check(e->level == depth, "preorder visit at level %d, expected %d", e->level, depth);
			pc_check(i + 2 < nev && !(evs[i + 1].v == postorder && evs[i + 1].node == e->node &&
			    evs[i + 2].v == endorder && evs[i + 2].node == e->node),
			    "a node without children was not visited as a leaf");
			stk[depth] = e->node;
			seen[depth] = 0;
			depth++;
			break;
		case postorder:
			pc_check(depth > 0 && e->level == depth - 1 && stk[depth - 1] == e->node && !seen[depth - 1],
			    "postorder visit out of place (level %d, depth %d)", e->level, depth);
			seen[depth - 1] = 1;
			emit = 1;
			break;
		case endorder:
			pc_check(depth > 0 && e->level == depth - 1 && stk[depth - 1] == e->node && seen[depth - 1],
			    "endorder visit out of place (level %d, depth %d)", e->level, depth);
			depth--;
			break;
		case leaf:
			pc_check(e->level == depth, "leaf visit at level %d, expected %d", e->level, depth);
			emit = 1;
			break;
		default:
			pc_fail("unknown VISIT value %d", (int)e->v);
		}
		if (emit) {
			pc_check(nk < maxkeys, "more elements than expected");
			keys[nk] = elem(e->node);
			if (nk > 0)
				pc_check(keys[nk - 1] < keys[nk], "elements not ascending: %d then %d", keys[nk - 1], keys[nk]);
			nk++;
		}
	}
	pc_check(depth == 0, "the walk ended with %d nodes still open", depth);
	pc_check(evs[0].level == 0, "the first visit is at level %d, not 0", evs[0].level);
	return nk;
}

static unsigned long rseed = 4242;

static unsigned rnd(void)
{
	rseed = rseed * 6364136223846793005UL + 1442695040888963407UL;
	return (unsigned)(rseed >> 33);
}

/* Fill p[0..n) with a pseudo-random permutation of 0..n-1. */
static void permute(int *p, int n)
{
	int i, j, t;

	for (i = 0; i < n; i++)
		p[i] = i;
	for (i = n - 1; i > 0; i--) {
		j = (int)(rnd() % (unsigned)(i + 1));
		t = p[i];
		p[i] = p[j];
		p[j] = t;
	}
}

#define N 40

int main(void)
{
	int v[N], keys[N + 1], order[N], del[N];
	void *root = NULL;
	int i, j;

	pc_begin("XSH.tsearch.005");
	for (i = 0; i < N; i++)
		v[i] = i * 2;
	permute(order, N);
	permute(del, N);
	for (i = 0; i < N; i++)
		pc_check(tsearch(&v[order[i]], &root, cmp) != NULL, "setup: tsearch() failed");
	pc_test();
	for (i = 0; i < N; i++) {
		int k = v[del[i]], nk, m;
		void *r;

		/* A key that is not present leaves the tree alone. */
		m = k + 1;
		pc_check(tdelete(&m, &root, cmp) == NULL, "tdelete() of absent key %d returned a pointer", m);
		r = tdelete(&k, &root, cmp);
		pc_check(r != NULL, "tdelete() of present key %d returned a null pointer", k);
		pc_check(tfind(&k, &root, cmp) == NULL, "key %d is still found after tdelete()", k);
		pc_check(tdelete(&k, &root, cmp) == NULL, "a second tdelete() of %d found it again", k);
		if (i < N - 1) {
			pc_check(root != NULL, "the root variable became null with %d elements left", N - 1 - i);
			nk = walk_check(root, keys, N + 1);
			pc_check(nk == N - 1 - i, "after %d deletions %d elements remain, expected %d", i + 1, nk, N - 1 - i);
			for (j = i + 1; j < N; j++) {
				int q = v[del[j]];

				pc_check(tfind(&q, &root, cmp) != NULL, "key %d was lost by deleting %d", q, k);
			}
		} else
			pc_check(root == NULL, "the root variable is not null after the last element was deleted");
	}
	pc_pass();
}
