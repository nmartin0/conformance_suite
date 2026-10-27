/*
 * be16toh: result types.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XSH.be16toh.006
 */
#include "pc.h"
#include <endian.h>
#include <stdint.h>

#define TY(fn, T, arg) do { \
	volatile T x_ = (arg); \
	pc_check(_Generic(fn(x_), T: 1, default: 0), #fn " does not return " #T); \
	pc_check(fn(x_) > 0, #fn " of a non-zero value gave a non-positive result"); \
} while (0)

int main(void)
{
	pc_begin("XSH.be16toh.006");
	pc_test();
	TY(be16toh, uint16_t, 0x0100); TY(htobe16, uint16_t, 0x0001); TY(htole16, uint16_t, 0x0001); TY(le16toh, uint16_t, 0x8000);
	TY(be32toh, uint32_t, 0x01000000u); TY(htobe32, uint32_t, 1u); TY(htole32, uint32_t, 1u); TY(le32toh, uint32_t, 0x80000000u);
	TY(be64toh, uint64_t, 0x0100000000000000ull); TY(htobe64, uint64_t, 1ull); TY(htole64, uint64_t, 1ull); TY(le64toh, uint64_t, 0x8000000000000000ull);
	pc_pass();
}
