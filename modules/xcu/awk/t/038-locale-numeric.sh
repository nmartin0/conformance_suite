# awk writes the radix character of the locale that LC_NUMERIC (or LC_ALL, or LANG) names when it converts a number to text.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.038
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.awk.038
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_probe() {
	awk 'BEGIN { x = 1.5; print x }' 2>/dev/null
}
pcl_want() {
	case $1 in
	L) printf '1,5' ;;
	C) printf '1.5' ;;
	esac
}
pc_test
pcloc_matrix "$PCLOC" LC_NUMERIC
pc_pass
