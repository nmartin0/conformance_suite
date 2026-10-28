# .i operand processed as C
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cflow.008
# A file with the suffix .i is taken as the output of preprocessing C source
# and gives the same call graph.
. "$PC_CORE/pc.sh"
pc_begin XCU.cflow.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >flow.c <<'EOS'
int leaf(void);
int
top(void)
{
	return leaf();
}
int
leaf(void)
{
	return 1;
}
EOS
c17 -E flow.c </dev/null >flow.i 2>cc.err || pc_unresolved "setup: c17 -E failed"
[ -s flow.i ] || pc_unresolved "setup: c17 -E wrote nothing"
pc_test
cflow flow.c </dev/null >c.out 2>err || pc_fail "cflow of the source: nonzero status"
cflow flow.i </dev/null >i.out 2>err || pc_fail "cflow of the .i file: nonzero status"
sed -n 1p i.out | grep ' top:' >/dev/null || pc_fail "first line for the .i file is not top: $(cat i.out)"
grep ' leaf:' i.out >/dev/null || pc_fail "leaf is missing for the .i file: $(cat i.out)"
[ "$(wc -l <i.out | tr -d ' ')" -eq "$(wc -l <c.out | tr -d ' ')" ] || pc_fail "the .i graph has a different number of lines"
pc_pass
