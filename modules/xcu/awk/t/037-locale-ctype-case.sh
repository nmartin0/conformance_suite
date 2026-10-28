# awk toupper and the character classes of its regular expressions follow the locale that LC_CTYPE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.037
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.awk.037
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
pcl_probe() {
	pcl_a=$(printf '\341\n' | awk '{ print toupper($0) }' 2>/dev/null | pcloc_hex)
	pcl_b=$(printf '\341\n' | awk '/^[[:lower:]]$/ { print "y" }' 2>/dev/null)
	printf '%s|%s' "$pcl_a" "$pcl_b"
}
pcl_want() {
	printf 'c10a|y'
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE
pc_pass
