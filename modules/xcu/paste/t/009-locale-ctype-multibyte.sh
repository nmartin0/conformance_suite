# paste takes a multibyte character of the code set that LC_CTYPE (or LC_ALL, or LANG) names as one delimiter.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.paste.009
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.paste.009
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make_mb
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
printf 'a\nb\n' >p1 || pc_unresolved "cannot write the input"
printf 'c\nd\n' >p2 || pc_unresolved "cannot write the input"
# the delimiter list holds one character of two bytes
pcl_probe() {
	pcl_dl=$(printf '\341\342')
	paste -d "$pcl_dl" p1 p2 2>/dev/null | pcloc_hex
}
pcl_want() {
	printf '61e1e2630a62e1e2640a'
}
pc_test
pcloc_matrix "$PCLOCMB" LC_CTYPE
pc_pass
