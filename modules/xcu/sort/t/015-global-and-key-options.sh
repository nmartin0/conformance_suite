# Global ordering options apply to keys; key options override them.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.015
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.015
# rd FILE: the content of FILE in rd_v, read by the shell alone (no other
# utility), each line followed by a newline.
rd() {
	rd_v=
	while IFS= read -r rd_l || [ -n "$rd_l" ]; do
		rd_v="$rd_v$rd_l
"
	done <"$1"
}
LC_ALL=POSIX
export LC_ALL
printf 'a 1\na 2\nb 1\n' >in1 || pc_unresolved "cannot create input"
printf 'a 10\na 9\nb 1\n' >in2 || pc_unresolved "cannot create input"
pc_test
sort -r -k1,1 -k2,2 in1 >out 2>err </dev/null || pc_fail "sort -r -k1,1 -k2,2: nonzero status"
rd out
[ "$rd_v" = "b 1
a 2
a 1
" ] || pc_fail "global -r was not applied to both keys: $rd_v"
sort -r -k1,1 -k2,2n in2 >out 2>err </dev/null || pc_fail "sort -r -k1,1 -k2,2n: nonzero status"
rd out
[ "$rd_v" = "b 1
a 9
a 10
" ] || pc_fail "the key option n did not override the global -r: $rd_v"
pc_pass
