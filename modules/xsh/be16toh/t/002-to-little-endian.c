/*
 * be16toh: to little endian.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.be16toh.002
 */
#include "pc.h"
#include <endian.h>
#include <stdint.h>
#include <string.h>

#define HB(fn, T, in, ...) do { \
	volatile T x_ = (in); \
	T r_ = fn(x_); \
	unsigned char b_[sizeof(T)]; \
	static const unsigned char e_[] = { __VA_ARGS__ }; \
	memcpy(b_, &r_, sizeof r_); \
	pc_check(sizeof e_ == sizeof b_ && memcmp(b_, e_, sizeof b_) == 0, #fn " gave the wrong bytes"); \
} while (0)

int main(void)
{
	pc_begin("XSH.be16toh.002");
	pc_test();
	HB(htole16, uint16_t, 0x0102, 0x02, 0x01);
	HB(htole16, uint16_t, 0x80ff, 0xff, 0x80);
	HB(htole32, uint32_t, 0x01020304u, 0x04, 0x03, 0x02, 0x01);
	HB(htole32, uint32_t, 0xf0e1d2c3u, 0xc3, 0xd2, 0xe1, 0xf0);
	HB(htole64, uint64_t, 0x0102030405060708ull, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01);
	HB(htole64, uint64_t, 0xfedcba9876543210ull, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe);
	pc_pass();
}
