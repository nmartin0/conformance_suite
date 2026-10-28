# nm identifying lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.005
# Without -A, "nm -P x.o y.o" prints "x.o:" and "y.o:" lines before each
# file's symbols, and "nm -P libx.a" for a library prints "libx.a[x.o]:"; a
# single non-library file has no such line.
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.005
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
printf 'int pcother(void) { return 2; }\n' >y.c
c17 -c y.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
ar -rc libx.a x.o </dev/null >/dev/null 2>&1 || pc_unresolved "setup: ar failed"
pc_test
nm -P x.o y.o >o 2>err </dev/null || pc_fail "nm -P x.o y.o: nonzero status"
grep -x -F 'x.o:' o >/dev/null || pc_fail "no x.o: line"
grep -x -F 'y.o:' o >/dev/null || pc_fail "no y.o: line"
nm -P x.o >o2 2>err </dev/null || pc_fail "nm -P x.o: nonzero status"
grep -x -F 'x.o:' o2 >/dev/null && pc_fail "a single object file got an identifying line"
nm -P libx.a >o3 2>err </dev/null || pc_fail "nm -P libx.a: nonzero status"
grep -x -F 'libx.a[x.o]:' o3 >/dev/null || pc_fail "no libx.a[x.o]: line for the library"
pc_pass
