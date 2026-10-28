# getconf reports, for each of the symbolic utility limits BC_BASE_MAX, BC_DIM_MAX, BC_SCALE_MAX, BC_STRING_MAX, COLL_WEIGHTS_MAX, EXPR_NEST_MAX, LINE_MAX and RE_DUP_MAX, an integer that is at least the stated minimum
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.2-utility-limits.001
. "$PC_CORE/pc.sh"
pc_begin XCU.1.2-utility-limits.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
chk() {
	v=$(getconf "$1" 2>/dev/null) || pc_fail "getconf $1 failed"
	case $v in
	'' | *[!0-9]*) pc_fail "getconf $1 gave '$v', not a number" ;;
	esac
	[ "$v" -ge "$2" ] || pc_fail "getconf $1 gave $v, below $2"
}
chk BC_BASE_MAX 99
chk BC_DIM_MAX 2048
chk BC_SCALE_MAX 99
chk BC_STRING_MAX 1000
chk COLL_WEIGHTS_MAX 2
chk EXPR_NEST_MAX 32
chk LINE_MAX 2048
chk RE_DUP_MAX 255
pc_pass
