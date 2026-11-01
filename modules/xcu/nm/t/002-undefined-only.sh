# nm -u
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.002
# "nm -u x.o" lists the undefined symbols and not the defined ones.
# MUTANT    nm.u_ignored
# MUTANT    nm.u_all
# MUTANT    nm.u_inverted
# MUTANT    nm.u_adds_text
# MUTANT    nm.u_first_only
# MUTANT    nm.u_none
# MUTANT    nm.u_as_g
# MUTANT    nm.status1
# MUTANT    nm.out_stderr
# VARIANT   nm.padded
# VARIANT   nm.prefixed
# VARIANT   nm.undef_zero
# VARIANT   nm.tabbed
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.002
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
extern int pcundefd;
int pcz1(void) { return 1; }
int pcy2(void) { return 2; }
int pcx3(void) { return 3; }
int pcw4(void) { return 4; }
int pcfunc(void) { return pclocal + pcundef() + pcundefd + pcdata[0] + pcbss[0]; }
EOC
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
pc_test
nm -uP x.o >o 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nm -uP: exit status $rc"
n=$(awk '$1 ~ /pcundef$/' o | wc -l)
[ "$((n + 0))" -eq 1 ] || pc_fail "the undefined function is not listed once"
n=$(awk '$1 ~ /pcundefd$/' o | wc -l)
[ "$((n + 0))" -eq 1 ] || pc_fail "the undefined data object is not listed once"
n=$(awk '$2 != "U"' o | wc -l)
[ "$((n + 0))" -eq 0 ] || pc_fail "-u lists a symbol that is not undefined: $(awk '$2 != "U"' o | head -n 1)"
n=$(awk '$1 ~ /pc(func|data|bss|local|z1|y2|x3|w4)$/' o | wc -l)
[ "$((n + 0))" -eq 0 ] || pc_fail "defined symbols are listed with -u"
pc_pass
