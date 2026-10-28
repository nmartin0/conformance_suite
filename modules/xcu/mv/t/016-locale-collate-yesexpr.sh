# The yesexpr that judges the answer of mv -i uses the collating elements of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.016
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.mv.016
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make y
pcl_lonly=1
# ch is a collating element of the locale and its yesexpr takes the response ch as affirmative
pcl_probe() {
	pcloc_yes mv ch
}
pcl_want() {
	printf Y
}
pc_test
pcloc_matrix "$PCLOCY" LC_COLLATE LC_MESSAGES
pc_pass
