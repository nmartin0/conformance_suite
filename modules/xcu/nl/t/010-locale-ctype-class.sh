# A character class in a nl regular expression follows the locale that LC_CTYPE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.010
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.nl.010
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make
pcl_basec="LC_COLLATE LC_NUMERIC LC_TIME LC_MONETARY LC_MESSAGES"
pcl_lonly=1
{ printf '%s\n' -; printf '\341\n\301\n'; } >n2.txt || pc_unresolved "cannot write the input"
# only the line with byte 0xe1, a lowercase letter in the locale, matches, so it is the only numbered line
pcl_probe() {
	nl -b p'^[[:lower:]]$' n2.txt 2>/dev/null | sed -n '/^ *[[:digit:]]/s/^ *[[:digit:]]*[[:space:]]*//p' | pcloc_hex
}
pcl_want() {
	printf 'e10a'
}
pc_test
pcloc_matrix "$PCLOC" LC_CTYPE
pc_pass
