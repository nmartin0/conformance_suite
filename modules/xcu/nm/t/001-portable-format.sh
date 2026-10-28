# nm -P line format and symbol types
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.001
# "nm -P x.o" prints lines of name, type letter, value and size separated by
# blanks; a global function is type T, an initialised global is D, an
# uninitialised global is B or C-style common (any letter accepted for it is
# not tested), an unresolved reference is U.
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.001
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
nm -P x.o >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nm -P: exit status $rc"
[ -s o ] || pc_fail "nm -P wrote nothing"
awk '$2 != "U" && NF != 4 && $1 ~ /pc(data|func)$/ { bad = 1 } END { exit bad }' o || pc_fail "a line for a defined symbol does not have four fields"
t=$(awk '$1 ~ /pcfunc$/ { print $2 }' o)
[ "$t" = T ] || pc_fail "pcfunc has type '$t', expected T"
t=$(awk '$1 ~ /pcdata$/ { print $2 }' o)
[ "$t" = D ] || pc_fail "pcdata has type '$t', expected D"
t=$(awk '$1 ~ /pcundef$/ { print $2 }' o)
[ "$t" = U ] || pc_fail "pcundef has type '$t', expected U"
t=$(awk '$1 ~ /pcbss$/ { print $2 }' o)
[ -n "$t" ] || pc_fail "pcbss is not listed"
pc_pass
