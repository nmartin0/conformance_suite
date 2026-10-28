# The yesexpr that judges the answer of mv -i uses the character classes of the locale that LC_CTYPE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.017
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.mv.017
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY"
pcl_lonly=1
# byte 0xc1 is an uppercase letter in the locale, whose yesexpr takes an uppercase letter followed by ! as affirmative
pcl_probe() {
	pcloc_yes mv '\301!'
}
pcl_want() {
	printf Y
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE LC_MESSAGES
pc_pass
