# -o file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cxref.004
# -o sends the listing to the named file instead of standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.cxref.004
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
cxref x.c </dev/null >plain 2>err || pc_fail "cxref: nonzero status"
[ -s plain ] || pc_fail "cxref wrote nothing"
cxref -o listing x.c </dev/null >stdout.out 2>err || pc_fail "cxref -o: nonzero status"
[ -s listing ] || pc_fail "-o file is empty or missing"
[ -s stdout.out ] && pc_fail "-o still wrote to standard output"
cmp plain listing >/dev/null 2>&1 || pc_fail "the -o file differs from the standard output listing"
pc_pass
