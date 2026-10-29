# Nine -k options are accepted and significant in command-line order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.016
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.016
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
printf 'a a a a a a a a z\na a a a a a a a y\n' >in1 || pc_unresolved "cannot create input"
printf 'a b\nb a\n' >in2 || pc_unresolved "cannot create input"
pc_test
sort -k1,1 -k2,2 -k3,3 -k4,4 -k5,5 -k6,6 -k7,7 -k8,8 -k9,9 in1 >out 2>err </dev/null || pc_fail "sort with nine keys: nonzero status: $(cat err)"
rd out
[ "$rd_v" = "a a a a a a a a y
a a a a a a a a z
" ] || pc_fail "the ninth key was not significant: $rd_v"
sort -k2,2 -k1,1 in2 >out 2>err </dev/null || pc_fail "sort -k2,2 -k1,1: nonzero status"
rd out
[ "$rd_v" = "b a
a b
" ] || pc_fail "the keys were not applied in command-line order: $rd_v"
pc_pass
