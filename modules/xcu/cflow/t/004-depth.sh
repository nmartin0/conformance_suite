# -d cut-off depth and non-positive values
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cflow.004
# -d with a small depth cuts deeper callees from the graph, a large depth
# changes nothing, and a depth of zero or a negative number is ignored.
. "$PC_CORE/pc.sh"
pc_begin XCU.cflow.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >chain.c <<'EOS'
int f4(void);
int f3(void);
int f2(void);
int
f1(void)
{
	return f2();
}
int
f2(void)
{
	return f3();
}
int
f3(void)
{
	return f4();
}
int
f4(void)
{
	return 4;
}
EOS
pc_test
cflow chain.c </dev/null >full 2>err || pc_fail "cflow: nonzero status"
grep ' f4:' full >/dev/null || pc_fail "the full graph does not reach f4: $(cat full)"
cflow -d 1 chain.c </dev/null >d1 2>err || pc_fail "cflow -d 1: nonzero status"
grep ' f1:' d1 >/dev/null || pc_fail "-d 1 lost the top function"
grep ' f4:' d1 >/dev/null && pc_fail "-d 1 still shows f4: $(cat d1)"
cflow -d 100 chain.c </dev/null >d100 2>err || pc_fail "cflow -d 100: nonzero status"
cmp full d100 >/dev/null 2>&1 || pc_fail "-d 100 changed the graph"
cflow -d 0 chain.c </dev/null >d0 2>err || pc_fail "cflow -d 0: nonzero status"
cmp full d0 >/dev/null 2>&1 || pc_fail "-d 0 was not ignored"
cflow -d -3 chain.c </dev/null >dm 2>err || pc_fail "cflow -d -3: nonzero status"
cmp full dm >/dev/null 2>&1 || pc_fail "-d -3 was not ignored"
pc_pass
