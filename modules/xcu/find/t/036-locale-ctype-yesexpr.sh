# The patterns of find -name and the yesexpr that judges the answer of -ok use the character classes of the locale that LC_CTYPE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.036
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.find.036
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY "
pcl_lonly=1
mkdir fnc || pc_unresolved "cannot make a directory"
pcl_name=$(printf '\341')
: >"fnc/$pcl_name" || pc_unresolved "cannot make a file with a non-portable byte in its name"
: >fnc/1 || pc_unresolved "cannot make a file"
# byte 0xe1 is a lowercase letter and 0xc1 an uppercase letter in the locale; its yesexpr takes an uppercase
# letter followed by ! as affirmative
pcl_probe() {
	pcl_a=$(find fnc -type f -name '[[:lower:]]' 2>/dev/null | pcloc_hex)
	pcl_b=$(pcloc_yes find '\301!')
	printf '%s|%s' "$pcl_a" "$pcl_b"
}
pcl_want() {
	printf '666e632fe10a|Y'
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE LC_MESSAGES
pc_pass
