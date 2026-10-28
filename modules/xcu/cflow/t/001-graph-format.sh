# numbered, indented lines with name and location
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cflow.001
# Each output line starts with its line number as the reference number,
# callees are indented deeper than their callers, a definition from C source
# ends with the file name and line number in angle brackets, and by default
# only functions not starting with an underscore are shown.
. "$PC_CORE/pc.sh"
pc_begin XCU.cflow.001
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
col() { awk -v n="$2" 'index($0, " " n ":") > 0 { print index($0, " " n ":"); exit }' "$1"; }
pc_test
cflow flow.c </dev/null >out 2>err || pc_fail "cflow: nonzero status"
n=$(wc -l <out | tr -d ' ')
[ "$n" -ge 3 ] || pc_fail "expected at least three lines, got $n: $(cat out)"
i=0
while read first rest; do
	i=$((i + 1))
	[ "$first" = "$i" ] || pc_fail "line $i starts with $first, not its own number"
done <out
grep ' top:.*<flow.c 9>' out >/dev/null || pc_fail "no location <flow.c 9> for top"
grep ' mid:.*<flow.c 14>' out >/dev/null || pc_fail "no location <flow.c 14> for mid"
grep ' leaf:.*<flow.c 19>' out >/dev/null || pc_fail "no location <flow.c 19> for leaf"
ct=$(col out top); cm=$(col out mid); cl=$(col out leaf)
[ "$cm" -gt "$ct" ] || pc_fail "mid is not indented deeper than top"
[ "$cl" -gt "$ct" ] || pc_fail "leaf is not indented deeper than top"
grep 'g_data\|_under' out >/dev/null && pc_fail "data or underscore names shown by default"
pc_pass
