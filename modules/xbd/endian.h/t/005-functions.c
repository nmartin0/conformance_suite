/*
 * <endian.h> byte-order conversions: each is a function with the page's
 * prototype or a macro that can be called with that argument.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.endian.h.005
 *
 * Only <endian.h> and <stdint.h> (for the argument types, whose definition
 * by <endian.h> is assertion 004) are included, not pc.h, so a name the
 * header fails to provide stops the build, which the module reports as
 * FAIL. The page allows a function, a macro or both: a name that is a
 * macro is called, one that is not is checked as a function.
 */
#include <endian.h>
#include <stdint.h>	/* the argument types; that <endian.h> defines them is assertion 004 */

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

static volatile unsigned long long sink;

int main(void)
{
	pc_begin("XBD.endian.h.005");
	pc_test();
#ifdef be16toh
	sink = be16toh((uint16_t)1);
#else
	if (!IS_T(&(be16toh), uint16_t (*)(uint16_t)))
		pc_fail("be16toh is not declared as uint16_t be16toh(uint16_t)");
#endif
#ifdef be32toh
	sink = be32toh((uint32_t)1);
#else
	if (!IS_T(&(be32toh), uint32_t (*)(uint32_t)))
		pc_fail("be32toh is not declared as uint32_t be32toh(uint32_t)");
#endif
#ifdef be64toh
	sink = be64toh((uint64_t)1);
#else
	if (!IS_T(&(be64toh), uint64_t (*)(uint64_t)))
		pc_fail("be64toh is not declared as uint64_t be64toh(uint64_t)");
#endif
#ifdef htobe16
	sink = htobe16((uint16_t)1);
#else
	if (!IS_T(&(htobe16), uint16_t (*)(uint16_t)))
		pc_fail("htobe16 is not declared as uint16_t htobe16(uint16_t)");
#endif
#ifdef htobe32
	sink = htobe32((uint32_t)1);
#else
	if (!IS_T(&(htobe32), uint32_t (*)(uint32_t)))
		pc_fail("htobe32 is not declared as uint32_t htobe32(uint32_t)");
#endif
#ifdef htobe64
	sink = htobe64((uint64_t)1);
#else
	if (!IS_T(&(htobe64), uint64_t (*)(uint64_t)))
		pc_fail("htobe64 is not declared as uint64_t htobe64(uint64_t)");
#endif
#ifdef htole16
	sink = htole16((uint16_t)1);
#else
	if (!IS_T(&(htole16), uint16_t (*)(uint16_t)))
		pc_fail("htole16 is not declared as uint16_t htole16(uint16_t)");
#endif
#ifdef htole32
	sink = htole32((uint32_t)1);
#else
	if (!IS_T(&(htole32), uint32_t (*)(uint32_t)))
		pc_fail("htole32 is not declared as uint32_t htole32(uint32_t)");
#endif
#ifdef htole64
	sink = htole64((uint64_t)1);
#else
	if (!IS_T(&(htole64), uint64_t (*)(uint64_t)))
		pc_fail("htole64 is not declared as uint64_t htole64(uint64_t)");
#endif
#ifdef le16toh
	sink = le16toh((uint16_t)1);
#else
	if (!IS_T(&(le16toh), uint16_t (*)(uint16_t)))
		pc_fail("le16toh is not declared as uint16_t le16toh(uint16_t)");
#endif
#ifdef le32toh
	sink = le32toh((uint32_t)1);
#else
	if (!IS_T(&(le32toh), uint32_t (*)(uint32_t)))
		pc_fail("le32toh is not declared as uint32_t le32toh(uint32_t)");
#endif
#ifdef le64toh
	sink = le64toh((uint64_t)1);
#else
	if (!IS_T(&(le64toh), uint64_t (*)(uint64_t)))
		pc_fail("le64toh is not declared as uint64_t le64toh(uint64_t)");
#endif
	(void)sink;
	pc_pass();
}
