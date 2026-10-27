/*
 * be16toh: from little endian.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.be16toh.004
 */
#include "pc.h"
#include <endian.h>
#include <stdint.h>
#include <string.h>

#define FB(fn, T, want, ...) do { \
	static const unsigned char e_[] = { __VA_ARGS__ }; \
	volatile T x_; \
	T r_; \
	T in_; \
	memcpy(&in_, e_, sizeof in_); \
	x_ = in_; \
	r_ = fn(x_); \
	pc_check(sizeof e_ == sizeof in_ && r_ == (want), #fn " gave the wrong value"); \
} while (0)

int main(void)
{
	pc_begin("XSH.be16toh.004");
	pc_test();
	FB(le16toh, uint16_t, 0x0102, 0x02, 0x01);
	FB(le16toh, uint16_t, 0x80ff, 0xff, 0x80);
	FB(le32toh, uint32_t, 0x01020304u, 0x04, 0x03, 0x02, 0x01);
	FB(le32toh, uint32_t, 0xf0e1d2c3u, 0xc3, 0xd2, 0xe1, 0xf0);
	FB(le64toh, uint64_t, 0x0102030405060708ull, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01);
	FB(le64toh, uint64_t, 0xfedcba9876543210ull, 0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe);
	pc_pass();
}
