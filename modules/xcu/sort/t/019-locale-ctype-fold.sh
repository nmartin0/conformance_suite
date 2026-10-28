# sort -f folds a lowercase letter of the locale that LC_CTYPE (or LC_ALL, or LANG) names to its uppercase equivalent.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.019
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.sort.019
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
pcl_count() {
	pcl_c=0
	while read -r pcl_x
	do
		pcl_c=$((pcl_c + 1))
	done
	printf '%s' "$pcl_c"
}
# In the locale byte 0xe1 is a lowercase letter whose uppercase equivalent is 0xc1: with -f the two lines have equal keys.
pcl_probe() {
	printf '\341\n\301\n' | sort -f -u 2>/dev/null | pcl_count
}
pcl_want() {
	printf 1
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE
pc_pass
