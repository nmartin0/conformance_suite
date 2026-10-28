# -i x for data and -i _ for underscore names
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cflow.005
# -i x adds external and static data symbols to the graph, -i _ adds names
# that begin with an underscore, and each is absent without its option.
. "$PC_CORE/pc.sh"
pc_begin XCU.cflow.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >flow.c <<'EOS'
int g_data;
int _under(void)
{
	return 0;
}
int leaf(void);
int mid(void);
int
top(void)
{
	return mid() + leaf();
}
int
mid(void)
{
	return leaf();
}
int
leaf(void)
{
	return g_data + _under();
}
EOS
pc_test
cflow flow.c </dev/null >plain 2>err || pc_fail "cflow: nonzero status"
grep 'g_data\|_under' plain >/dev/null && pc_fail "default graph shows data or underscore names"
cflow -i x flow.c </dev/null >ix 2>err || pc_fail "cflow -i x: nonzero status"
grep ' g_data:.*<flow.c 1>' ix >/dev/null || pc_fail "-i x does not show g_data with its location: $(cat ix)"
grep '_under' ix >/dev/null && pc_fail "-i x alone shows an underscore name"
cflow -i _ flow.c </dev/null >iu 2>err || pc_fail "cflow -i _: nonzero status"
grep ' _under:.*<flow.c 2>' iu >/dev/null || pc_fail "-i _ does not show _under with its location: $(cat iu)"
grep 'g_data' iu >/dev/null && pc_fail "-i _ alone shows a data symbol"
cflow -i x -i _ flow.c </dev/null >ixu 2>err || pc_fail "cflow -i x -i _: nonzero status"
grep ' g_data:' ixu >/dev/null || pc_fail "-i x -i _ lost g_data"
grep ' _under:' ixu >/dev/null || pc_fail "-i x -i _ lost _under"
pc_pass
