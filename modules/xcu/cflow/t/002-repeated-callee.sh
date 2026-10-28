# later mention of a described name is a reference number
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cflow.002
# Once a name has been written with its definition, a later mention of it
# carries the reference number of that line and no definition.
. "$PC_CORE/pc.sh"
pc_begin XCU.cflow.002
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
cflow flow.c </dev/null >out 2>err || pc_fail "cflow: nonzero status"
n1=$(awk '/ leaf:.*<flow.c 19>/ { print $1; exit }' out)
[ -n "$n1" ] || pc_fail "no defining line for leaf: $(cat out)"
grep ' leaf:' out | grep -v '<' >refs
[ -s refs ] || pc_fail "no later mention of leaf without a definition: $(cat out)"
while read -r first rest; do
	printf '%s\n' "$rest" | grep -w "$n1" >/dev/null || pc_fail "a later mention of leaf does not give reference number $n1: $first $rest"
done <refs
pc_pass
