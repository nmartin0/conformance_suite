/*
 * <termios.h> functions
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.termios.h.018
 *
 * Only <termios.h> is included (not pc.h, which brings in <stddef.h>), so a name
 * the header fails to provide stops the build, which the module reports as FAIL.
 */
#include <termios.h>

/* The harness, declared here so that no other header is included. */
void pc_begin(const char *);
void pc_test(void);
_Noreturn void pc_pass(void);
_Noreturn void pc_fail(const char *, ...);

/* 1 if expression e has type T (after lvalue conversion). */
#define IS_T(e, T) _Generic((e), T: 1, default: 0)

int main(void)
{
	pc_begin("XBD.termios.h.018");
	pc_test();
	if (!IS_T(&(cfgetispeed), speed_t (*)(const struct termios *)))
		pc_fail("cfgetispeed() is not declared as speed_t cfgetispeed(const struct termios *)");
	if (IS_T(&(cfgetispeed), speed_t (*)(const struct termios *, int)))
		pc_fail("cfgetispeed() is declared without a prototype");
	if (!IS_T(&(cfgetospeed), speed_t (*)(const struct termios *)))
		pc_fail("cfgetospeed() is not declared as speed_t cfgetospeed(const struct termios *)");
	if (IS_T(&(cfgetospeed), speed_t (*)(const struct termios *, int)))
		pc_fail("cfgetospeed() is declared without a prototype");
	if (!IS_T(&(cfsetispeed), int (*)(struct termios *, speed_t)))
		pc_fail("cfsetispeed() is not declared as int cfsetispeed(struct termios *, speed_t)");
	if (IS_T(&(cfsetispeed), int (*)(struct termios *, speed_t, int)))
		pc_fail("cfsetispeed() is declared without a prototype");
	if (!IS_T(&(cfsetospeed), int (*)(struct termios *, speed_t)))
		pc_fail("cfsetospeed() is not declared as int cfsetospeed(struct termios *, speed_t)");
	if (IS_T(&(cfsetospeed), int (*)(struct termios *, speed_t, int)))
		pc_fail("cfsetospeed() is declared without a prototype");
	if (!IS_T(&(tcdrain), int (*)(int)))
		pc_fail("tcdrain() is not declared as int tcdrain(int)");
	if (IS_T(&(tcdrain), int (*)(int, int)))
		pc_fail("tcdrain() is declared without a prototype");
	if (!IS_T(&(tcflow), int (*)(int, int)))
		pc_fail("tcflow() is not declared as int tcflow(int, int)");
	if (IS_T(&(tcflow), int (*)(int, int, int)))
		pc_fail("tcflow() is declared without a prototype");
	if (!IS_T(&(tcflush), int (*)(int, int)))
		pc_fail("tcflush() is not declared as int tcflush(int, int)");
	if (IS_T(&(tcflush), int (*)(int, int, int)))
		pc_fail("tcflush() is declared without a prototype");
	if (!IS_T(&(tcgetattr), int (*)(int, struct termios *)))
		pc_fail("tcgetattr() is not declared as int tcgetattr(int, struct termios *)");
	if (IS_T(&(tcgetattr), int (*)(int, struct termios *, int)))
		pc_fail("tcgetattr() is declared without a prototype");
	if (!IS_T(&(tcgetsid), pid_t (*)(int)))
		pc_fail("tcgetsid() is not declared as pid_t tcgetsid(int)");
	if (IS_T(&(tcgetsid), pid_t (*)(int, int)))
		pc_fail("tcgetsid() is declared without a prototype");
	if (!IS_T(&(tcsendbreak), int (*)(int, int)))
		pc_fail("tcsendbreak() is not declared as int tcsendbreak(int, int)");
	if (IS_T(&(tcsendbreak), int (*)(int, int, int)))
		pc_fail("tcsendbreak() is declared without a prototype");
	if (!IS_T(&(tcsetattr), int (*)(int, int, const struct termios *)))
		pc_fail("tcsetattr() is not declared as int tcsetattr(int, int, const struct termios *)");
	if (IS_T(&(tcsetattr), int (*)(int, int, const struct termios *, int)))
		pc_fail("tcsetattr() is declared without a prototype");
	pc_pass();
}
