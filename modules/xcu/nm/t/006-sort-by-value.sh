# nm -v ordering
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.006
# By default the output is sorted by symbol name (POSIX locale); with -v the
# defined symbols appear in nondecreasing order of value.
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.006
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
nm -P x.o >o 2>err </dev/null || pc_fail "nm -P: nonzero status"
names=$(awk '$1 ~ /pc[wxyz][1-4]$/ { print substr($1, length($1) - 3) }' o | tr '\n' ' ')
[ "$names" = 'pcw4 pcx3 pcy2 pcz1 ' ] || pc_fail "default order is '$names', expected sorted by name"
nm -vP -t d x.o >o2 2>err </dev/null || pc_fail "nm -vP -t d: nonzero status"
awk '$1 ~ /pc[wxyz][1-4]$/ { n++; if (n > 1 && $3 + 0 < prev) bad = 1; prev = $3 + 0 } END { exit (bad || n != 4) }' o2 || pc_fail "-v did not give nondecreasing values for the four functions"
pc_pass
