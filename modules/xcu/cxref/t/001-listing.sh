# file name line, symbols, declaring asterisk, function and lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cxref.001
# The listing starts with the input file name on its own line, then has an
# entry for auto, static and global symbols and for macros from #define,
# each giving its line numbers with an asterisk before the declaring one and
# the name of the function it appears in.
. "$PC_CORE/pc.sh"
pc_begin XCU.cxref.001
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
blk() { awk -v s="$2" '/^[^ \t]/ { cur = ($1 == s) } cur { print }' "$1"; }
star() { grep -E "\\*[[:blank:]]*$2([^0-9]|\$)" "$1" >/dev/null; }
pc_test
cxref x.c </dev/null >out 2>err || pc_fail "cxref: nonzero status"
sed -n 1p out | grep 'x\.c' >/dev/null || pc_fail "first line does not name the file: $(sed -n 1p out)"
for s in LIM counter helper loc run; do
	blk out $s >b.$s
	if [ ! -s b.$s ]; then
		grep -w "$s" out >/dev/null || pc_fail "no entry for $s: $(cat out)"
		pc_unresolved "cannot find the entry for $s: the layout of the listing is unspecified"
	fi
done
star b.counter 2 || pc_fail "counter: no asterisk before its declaring line 2: $(cat b.counter)"
for n in 11 12 13; do grep -w "$n" b.counter >/dev/null || pc_fail "counter: line $n not listed"; done
grep -w run b.counter >/dev/null || pc_fail "counter: function run not named"
star b.helper 3 || pc_fail "helper: no asterisk before line 3"
grep -w 11 b.helper >/dev/null || pc_fail "helper: use on line 11 not listed"
star b.run 8 || pc_fail "run: no asterisk before line 8"
star b.LIM 1 || pc_fail "LIM: macro from #define not marked at line 1"
grep -w 5 b.LIM >/dev/null || pc_fail "LIM: use on line 5 not listed"
star b.loc 10 || pc_fail "loc: no asterisk before line 10"
for n in 11 12; do grep -w "$n" b.loc >/dev/null || pc_fail "loc: line $n not listed"; done
grep -w run b.loc >/dev/null || pc_fail "loc: function run not named"
pc_pass
