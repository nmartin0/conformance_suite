# The string comparison operators of expr use the collating sequence of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.expr.010
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.expr.010
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_probe() {
	expr b '<' a 2>/dev/null
}
pcl_want() {
	case $1 in
	L) printf 1 ;;
	C) printf 0 ;;
	esac
}
pc_test
pcloc_matrix "$PCLOC" LC_COLLATE
pc_pass
