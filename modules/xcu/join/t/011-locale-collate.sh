# join expects its files to be sorted in the collating sequence of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.011
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.join.011
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
printf 'b 1\na 2\n' >j1 || pc_fail "cannot write j1"
printf 'a 3\n' >j2 || pc_fail "cannot write j2"
printf 'a 2\nb 1\n' >k1 || pc_fail "cannot write k1"
printf 'b 3\n' >k2 || pc_fail "cannot write k2"
pcl_probe() {
	if [ "$1" = L ]; then
		join j1 j2
	else
		join k1 k2
	fi 2>/dev/null
}
pcl_want() {
	case $1 in
	L) printf 'a 2 3' ;;
	C) printf 'b 1 3' ;;
	esac
}
pc_test
pcloc_matrix "$PCLOC" LC_COLLATE
pc_pass
