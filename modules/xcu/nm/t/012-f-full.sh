# nm -f lists at least what the default lists
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.012
# "nm -f" lists at least every symbol name that the default output lists
# (defined, static and undefined ones, in the same format).
# MUTANT    nm.f_drops_local
# MUTANT    nm.f_drops_undef
# MUTANT    nm.f_drops_data
# MUTANT    nm.f_drops_text
# MUTANT    nm.f_first_only
# MUTANT    nm.f_none
# MUTANT    nm.f_default_format
# MUTANT    nm.status1
# MUTANT    nm.out_stderr
# VARIANT   nm.f_extra
# VARIANT   nm.padded
# VARIANT   nm.prefixed
# VARIANT   nm.tabbed
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_notinuse "c17 is not available"
cat >x.c <<'EOC'
static int pclocal = 5;
static int pcstat(void) { return 8; }
extern int pcundef(void);
int pcglobal = 7;
int pcbss[4];
int pcfunc(void) { return pclocal + pcglobal + pcstat() + pcundef() + pcbss[0]; }
EOC
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
pc_test
nm -P x.o >o1 2>err </dev/null || pc_fail "nm -P: nonzero status"
nm -f -P x.o >o2 2>err </dev/null || pc_fail "nm -f -P: nonzero status: $(cat err)"
awk '$1 ~ /pcfunc$/ { f = 1 } END { exit !f }' o1 || pc_unresolved "pcfunc is not in the default output"
for s in pclocal pcstat pcundef pcglobal pcbss pcfunc; do
	awk -v s="$s" '$1 ~ (s "$") { f = 1 } END { exit !f }' o1 || pc_unresolved "$s is not in the default output"
done
awk 'NR == FNR { want[$1]; next } { got[$1] } END { for (k in want) if (!(k in got)) { print k; bad = 1 } exit bad }' o1 o2 >missing || pc_fail "nm -f lacks the symbol $(head -n 1 missing)"
pc_pass
