# The answer to the question of the -ok primary of find is judged by the yesexpr of the locale that LC_MESSAGES (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.034
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.find.034
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
# The locale takes x as affirmative and not y; the POSIX locale takes y and not x.
pcl_probe() {
	if [ "$1" = L ]; then pcl_a=x pcl_b=y; else pcl_a=y pcl_b=x; fi
	pcl_r1=$(pcloc_yes find "$pcl_a")
	pcl_r2=$(pcloc_yes find "$pcl_b")
	printf '%s%s' "$pcl_r1" "$pcl_r2"
}
pcl_want() {
	printf YN
}
pc_test
pcloc_matrix "$PCLOC" LC_MESSAGES
pc_pass
