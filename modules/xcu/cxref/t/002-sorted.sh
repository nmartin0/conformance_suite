# symbols are sorted
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cxref.002
# The symbols in the listing are sorted, in the collation sequence of the
# POSIX locale.
. "$PC_CORE/pc.sh"
pc_begin XCU.cxref.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >x.c <<'EOS'
#define LIM 10
int counter;
static int helper(int v)
{
	return v + LIM;
}
int
run(void)
{
	int loc;
	loc = helper(counter);
	counter = loc;
	return counter;
}
EOS
pc_test
cxref x.c </dev/null >out 2>err || pc_fail "cxref: nonzero status"
awk '/^[^ \t]/ && ($1 == "LIM" || $1 == "counter" || $1 == "helper" || $1 == "loc" || $1 == "run") { print $1 }' out >heads
if [ "$(wc -l <heads | tr -d ' ')" -lt 5 ]; then
	for s in LIM counter helper loc run; do
		grep -w "$s" out >/dev/null || pc_fail "no entry for $s: $(cat out)"
	done
	pc_unresolved "cannot find the five entries: the layout of the listing is unspecified"
fi
sort -c heads 2>/dev/null || pc_fail "symbols are not in sorted order: $(tr '\n' ' ' <heads)"
pc_pass
