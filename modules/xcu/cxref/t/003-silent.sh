# -s omits the file name line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cxref.003
# -s leaves out the line with the input file name.
. "$PC_CORE/pc.sh"
pc_begin XCU.cxref.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >x.c <<'EOS'
int counter;
int
run(void)
{
	return counter;
}
EOS
pc_test
cxref x.c </dev/null >full 2>err || pc_fail "cxref: nonzero status"
cxref -s x.c </dev/null >quiet 2>err || pc_fail "cxref -s: nonzero status"
[ -s quiet ] || pc_fail "cxref -s wrote nothing"
sed -n 1p full | grep 'x\.c' >/dev/null || pc_fail "without -s the first line does not name the file"
[ "$(sed -n 1p quiet)" != "$(sed -n 1p full)" ] || pc_fail "with -s the first line is still the one without -s: $(sed -n 1p quiet)"
[ "$(wc -l <quiet | tr -d ' ')" -lt "$(wc -l <full | tr -d ' ')" ] || pc_fail "-s did not shorten the listing"
pc_pass
