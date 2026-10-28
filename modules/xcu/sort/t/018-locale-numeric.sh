# sort -n takes the radix character from the locale that LC_NUMERIC (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.018
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.sort.018
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_count() {
	pcl_c=0
	while read -r pcl_x
	do
		pcl_c=$((pcl_c + 1))
	done
	printf '%s' "$pcl_c"
}
# With a comma as the radix character 2,25 and 2,5 are different numbers;
# with a period, the comma ends the number and both are 2.
pcl_probe() {
	printf '2,25\n2,5\n' | sort -n -u 2>/dev/null | pcl_count
}
pcl_want() {
	case $1 in
	L) printf 2 ;;
	C) printf 1 ;;
	esac
}
pc_test
pcloc_matrix "$PCLOC" LC_NUMERIC
pc_pass
