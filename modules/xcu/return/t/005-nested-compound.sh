# return inside loops, if and case commands of a function ends the function
# at once.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.return.005
. "$PC_CORE/pc.sh"
pc_begin XCU.return.005
pc_test
pcf() {
	for i in 1 2 3; do
		case $i in
		2) if true; then return 7; fi ;;
		esac
		echo "$i"
	done
	echo notrun
}
pcf >pc.fo
rc=$?
[ "$rc" -eq 7 ] || pc_fail "status $rc, expected 7"
[ "$(cat pc.fo)" = 1 ] || pc_fail "output was '$(cat pc.fo)', expected 1"
pc_pass
