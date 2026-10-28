# comm expects its files to be sorted in the collating sequence of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.comm.006
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.comm.006
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
printf 'b\na\n' >l1 || pc_fail "cannot write l1"
printf 'a\n' >l2 || pc_fail "cannot write l2"
printf 'a\nb\n' >c1 || pc_fail "cannot write c1"
printf 'b\n' >c2 || pc_fail "cannot write c2"
# l1 is in the order of the locale and c1 in the POSIX order; each pair has one line in common.
pcl_probe() {
	if [ "$1" = L ]; then
		comm -12 l1 l2
	else
		comm -12 c1 c2
	fi 2>/dev/null
}
pcl_want() {
	case $1 in
	L) printf 'a' ;;
	C) printf 'b' ;;
	esac
}
pc_test
pcloc_matrix "$PCLOC" LC_COLLATE
pc_pass
