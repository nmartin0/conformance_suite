/*
 * atomic_compare_exchange_strong_explicit() and _weak_explicit() behave
 * as the plain forms for every allowed (success, failure) memory order
 * pair, in both outcomes, for every atomic integer type.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.atomic_compare_exchange_strong.004
 */
#define PCAT_ID "XSH.atomic_compare_exchange_strong.004"
#include "pcat.h"
#ifndef PCAT_NONE

/* failure: neither release nor acq_rel, and not stronger than success */
#define R memory_order_relaxed
#define C memory_order_consume
#define A memory_order_acquire
#define L memory_order_release
#define Q memory_order_acq_rel
#define S memory_order_seq_cst
static const struct { memory_order succ, fail; } pairs[] = {
	{ R, R },
	{ C, R }, { C, C },
	{ A, R }, { A, C }, { A, A },
	{ L, R },
	{ Q, R }, { Q, C }, { Q, A },
	{ S, R }, { S, C }, { S, A }, { S, S }
};
#define NPAIRS (int)(sizeof pairs / sizeof pairs[0])

#define CASE(tag, ct, mn, mx) \
static void run_##tag(void) \
{ \
	ct vals[4] = { (ct)0, (ct)1, (ct)(mn), (ct)(mx) }; \
	int i, k, w, tries; \
	for (k = 0; k < NPAIRS; k++) { \
		memory_order so = pairs[k].succ, fo = pairs[k].fail; \
		for (i = 0; i < 4; i++) { \
			for (w = 0; w < 2; w++) { \
				ct des = vals[(i + 1) % 4], other = vals[(i + 2) % 4], now, exp; \
				atomic_##tag obj = vals[i]; \
				atomic_##tag *volatile p = &obj; \
				ct *volatile pe = &exp; \
				_Bool r = 0; \
				/* equal: success (the weak form may need retries) */ \
				for (tries = 0; tries < 100000; tries++) { \
					exp = vals[i]; \
					r = w ? atomic_compare_exchange_weak_explicit(p, pe, des, so, fo) \
					      : atomic_compare_exchange_strong_explicit(p, pe, des, so, fo); \
					if (r || !w) \
						break; \
				} \
				if (!r && w) \
					pc_unresolved("atomic_" #tag " weak exchange never succeeded in 100000 tries"); \
				now = *p; \
				pc_check(r, "atomic_" #tag " %s %s/%s: false for equal values", w ? "weak" : "strong", \
				    pcat_oname(so), pcat_oname(fo)); \
				pc_check(now == des && exp == vals[i], \
				    "atomic_" #tag " %s %s/%s: success left object %llx, *expected %llx", \
				    w ? "weak" : "strong", pcat_oname(so), pcat_oname(fo), PCAT_V(now), PCAT_V(exp)); \
				/* different: failure */ \
				exp = other; \
				r = w ? atomic_compare_exchange_weak_explicit(p, pe, vals[i], so, fo) \
				      : atomic_compare_exchange_strong_explicit(p, pe, vals[i], so, fo); \
				now = *p; \
				pc_check(other != des, "atomic_" #tag ": test values coincide"); \
				pc_check(!r, "atomic_" #tag " %s %s/%s: true for different values", w ? "weak" : "strong", \
				    pcat_oname(so), pcat_oname(fo)); \
				pc_check(now == des && exp == des, \
				    "atomic_" #tag " %s %s/%s: failure left object %llx, *expected %llx (both should be %llx)", \
				    w ? "weak" : "strong", pcat_oname(so), pcat_oname(fo), PCAT_V(now), PCAT_V(exp), PCAT_V(des)); \
			} \
		} \
	} \
}
PCAT_ALL(CASE)
#undef CASE

int main(void)
{
	pc_begin(PCAT_ID);
	pc_test();
#define CALL(tag, ct, mn, mx) run_##tag();
	PCAT_ALL(CALL)
#undef CALL
	pc_pass();
}
#endif
