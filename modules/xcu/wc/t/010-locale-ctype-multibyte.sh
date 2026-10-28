# wc -m counts the characters of the code set that LC_CTYPE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.010
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.wc.010
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make_mb
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
# a two-byte character between two letters, and the newline: four characters
pcl_probe() {
	pcl_o=$(printf 'a\341\342b\n' | wc -m 2>/dev/null)
	set -- $pcl_o
	printf '%s' "$1"
}
pcl_want() {
	printf 4
}
pc_test
pcloc_matrix "$PCLOCMB" LC_CTYPE
pc_pass
