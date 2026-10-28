# nm -A prefix
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.004
# With -A -P each line starts with the file operand followed by a colon and
# a blank; for a library the member name follows the library in square
# brackets.
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >x.c <<'EOC'
int pcdata[4] = {1, 2, 3, 4};
int pcbss[8];
static int pclocal = 5;
extern int pcundef(void);
int pcz1(void) { return 1; }
int pcy2(void) { return 2; }
int pcx3(void) { return 3; }
int pcw4(void) { return 4; }
int pcfunc(void) { return pclocal + pcundef() + pcdata[0] + pcbss[0]; }
EOC
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
ar -rc libx.a x.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
pc_test
nm -AP x.o >o 2>err </dev/null || pc_fail "nm -AP x.o: nonzero status"
[ -s o ] || pc_fail "nm -AP wrote nothing"
n=$(awk '$1 != "x.o:"' o | wc -l)
[ "$((n + 0))" -eq 0 ] || pc_fail "a line does not start with the file name and a colon"
nm -AP libx.a >o2 2>err </dev/null || pc_fail "nm -AP libx.a: nonzero status"
[ -s o2 ] || pc_fail "nm -AP of a library wrote nothing"
n=$(awk '$1 != "libx.a[x.o]:"' o2 | wc -l)
[ "$((n + 0))" -eq 0 ] || pc_fail "a library line does not start with libx.a[x.o]:"
pc_pass
