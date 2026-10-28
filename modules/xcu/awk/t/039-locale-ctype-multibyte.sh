# awk counts characters, not bytes, in the code set of the locale that LC_CTYPE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.039
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.awk.039
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make_mb
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
# a two-byte character between two letters is one character
pcl_probe() {
	printf 'a\341\342b\n' | awk '{ print length($0) }' 2>/dev/null
}
pcl_want() {
	printf 3
}
pc_test
pcloc_matrix "$PCLOCMB" LC_CTYPE
pc_pass
