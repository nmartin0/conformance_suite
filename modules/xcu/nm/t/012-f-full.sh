# nm -f lists at least what the default lists
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.012
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
int pcglobal = 7;
int pcfunc(void) { return pclocal + pcglobal; }
EOC
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
pc_test
nm -P x.o >o1 2>err </dev/null || pc_fail "nm -P: nonzero status"
nm -f -P x.o >o2 2>err </dev/null || pc_fail "nm -f -P: nonzero status: $(cat err)"
awk '$1 ~ /pcfunc$/ { f = 1 } END { exit !f }' o1 || pc_unresolved "pcfunc is not in the default output"
awk '{ print $1 }' o1 | while read -r s; do
	awk -v s="$s" '$1 == s { f = 1 } END { exit !f }' o2 || { echo "$s" >missing; }
done
[ -e missing ] && pc_fail "nm -f lacks the symbol $(cat missing)"
pc_pass
