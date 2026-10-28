# nm -x and -o
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.008
# "nm -x x.o" prints the same as "nm -t x x.o" and "nm -o x.o" the same as
# "nm -t o x.o".
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.008
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
pc_test
nm -x x.o >o1 2>err </dev/null || pc_fail "nm -x: nonzero status"
nm -t x x.o >o2 2>err </dev/null || pc_fail "nm -t x: nonzero status"
cmp -s o1 o2 || pc_fail "-x differs from -t x"
nm -o x.o >o3 2>err </dev/null || pc_fail "nm -o: nonzero status"
nm -t o x.o >o4 2>err </dev/null || pc_fail "nm -t o: nonzero status"
cmp -s o3 o4 || pc_fail "-o differs from -t o"
pc_pass
