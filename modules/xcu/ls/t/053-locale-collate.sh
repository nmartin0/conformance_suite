# ls orders names by the collating sequence of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.053
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.ls.053
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
mkdir lsd || pc_unresolved "cannot make a directory"
: >lsd/a || pc_unresolved "cannot make a file"
: >lsd/b || pc_unresolved "cannot make a file"
pcl_probe() {
	ls -1 lsd 2>/dev/null
}
pcl_want() {
	case $1 in
	L) printf 'b\na' ;;
	C) printf 'a\nb' ;;
	esac
}
pc_test
pcloc_matrix "$PCLOC" LC_COLLATE
pc_pass
