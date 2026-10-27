/*
 * be16toh: round trips.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.be16toh.005
 */
#include "pc.h"
#include <endian.h>
#include <stdint.h>
#include <string.h>

#define RT(N, T, v) do { \
	volatile T x_ = (v); \
	T be_ = htobe##N(x_), le_ = htole##N(x_); \
	unsigned char bb_[sizeof(T)], lb_[sizeof(T)]; \
	size_t i_; \
	pc_check(be##N##toh(be_) == (T)(v), "be" #N "toh(htobe" #N "(x)) is not x"); \
	pc_check(le##N##toh(le_) == (T)(v), "le" #N "toh(htole" #N "(x)) is not x"); \
	memcpy(bb_, &be_, sizeof be_); \
	memcpy(lb_, &le_, sizeof le_); \
	for (i_ = 0; i_ < sizeof bb_; i_++) \
		pc_check(bb_[i_] == lb_[sizeof lb_ - 1 - i_], "the bytes of htobe" #N "(x) are not those of htole" #N "(x) reversed"); \
} while (0)

int main(void)
{
	pc_begin("XSH.be16toh.005");
	pc_test();
	RT(16, uint16_t, 0); RT(16, uint16_t, 1); RT(16, uint16_t, 0xff); RT(16, uint16_t, 0x100);
	RT(16, uint16_t, 0x7fff); RT(16, uint16_t, 0x8000); RT(16, uint16_t, 0xffff);
	RT(32, uint32_t, 0); RT(32, uint32_t, 1); RT(32, uint32_t, 0x12345678u); RT(32, uint32_t, 0x80000000u); RT(32, uint32_t, 0xffffffffu);
	RT(64, uint64_t, 0); RT(64, uint64_t, 1); RT(64, uint64_t, 0x0123456789abcdefull); RT(64, uint64_t, 0x8000000000000000ull); RT(64, uint64_t, 0xffffffffffffffffull);
	pc_pass();
}
