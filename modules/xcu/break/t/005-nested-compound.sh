# break inside if, case and brace-group commands that are themselves inside
# a loop still leaves the loop.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.break.005
. "$PC_CORE/pc.sh"
pc_begin XCU.break.005
pc_test
r=
for i in 1 2 3; do
	case $i in
	2) { if true; then break; fi; } ;;
	esac
	r=$r$i
done
[ "$r" = 1 ] || pc_fail "got '$r', expected 1"
pc_pass
