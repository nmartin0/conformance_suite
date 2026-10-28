# ls -q leaves a character that the locale named by LC_CTYPE (or LC_ALL, or LANG) classes as printable alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.054
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.ls.054
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
mkdir lsq || pc_unresolved "cannot make a directory"
pcl_name=$(printf 'a\341b')
: >"lsq/$pcl_name" || pc_unresolved "cannot make a file with a non-portable byte in its name"
# byte 0xe1 is a lowercase letter, so a printable character, in the locale
pcl_probe() {
	ls -1 -q lsq 2>/dev/null | pcloc_hex
}
pcl_want() {
	printf '61e1620a'
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE
pc_pass
