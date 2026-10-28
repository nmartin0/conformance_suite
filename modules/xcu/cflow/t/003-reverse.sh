# -r lists callers sorted by callee
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cflow.003
# With -r the listing shows the callers of each function, indented under the
# callee, and the callees at the outermost level are in lexicographic order.
. "$PC_CORE/pc.sh"
pc_begin XCU.cflow.003
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
cflow -r flow.c </dev/null >out 2>err || pc_fail "cflow -r: nonzero status"
sed -n 1p out | grep ' leaf:' >/dev/null || pc_fail "first line of -r output is not leaf: $(sed -n 1p out)"
c1=$(awk 'NR == 1 { print index($0, " leaf:") }' out)
awk -v c="$c1" 'NR > 1 { p = match($0, /[ ][A-Za-z_][A-Za-z_0-9]*:/); if (p > 0 && p > c) { print $0 } }' out >nested
[ -s nested ] || pc_fail "no caller is indented under leaf"
grep ' mid:\| top:' nested >/dev/null || pc_fail "no caller of leaf listed under it"
# outermost names must be in ascending order
awk -v c="$c1" '{ p = match($0, /[ ][A-Za-z_][A-Za-z_0-9]*:/); if (p == c) { s = substr($0, p + 1); sub(/:.*/, "", s); print s } }' out >roots
sort -c roots 2>/dev/null || pc_fail "outermost callees are not sorted: $(cat roots)"
pc_pass
