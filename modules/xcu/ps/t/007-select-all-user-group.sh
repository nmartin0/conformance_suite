# -A, -U and -G select the test shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ps.007
. "$PC_CORE/pc.sh"
pc_begin XCU.ps.007
LC_ALL=C; export LC_ALL
bg=
trap '[ -n "$bg" ] && { kill "$bg" 2>/dev/null; wait "$bg" 2>/dev/null; }' EXIT
sleep 301 </dev/null >/dev/null 2>&1 &
bg=$!
ru=$(id -ur) || pc_fail "id -ur failed"
rg=$(id -gr) || pc_fail "id -gr failed"
# listed OPTION-ARGS...: run ps, set fa fb to whether $$ and $bg appear.
listed() {
	ps -o pid= "$@" >out 2>err </dev/null || pc_fail "ps $* failed"
	fa=no fb=no
	while IFS= read -r l; do
		set -- $l
		[ "$1" = "$$" ] && fa=yes
		[ "$1" = "$bg" ] && fb=yes
	done <out
}
pc_test
listed -A
[ "$fa" = yes ] && [ "$fb" = yes ] || pc_fail "ps -A did not list both $$ and $bg"
listed -U "$ru"
[ "$fa" = yes ] || pc_fail "ps -U $ru did not list $$"
listed -G "$rg"
[ "$fa" = yes ] || pc_fail "ps -G $rg did not list $$"
pc_pass
