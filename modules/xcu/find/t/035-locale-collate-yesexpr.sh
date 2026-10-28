# The patterns of find -name and the yesexpr that judges the answer of -ok use the collating elements of the locale that LC_COLLATE (or LC_ALL, or LANG) names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.035
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.find.035
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make y
pcl_lonly=1
mkdir fnd || pc_unresolved "cannot make a directory"
: >fnd/ch || pc_unresolved "cannot make a file"
: >fnd/c || pc_unresolved "cannot make a file"
: >fnd/h || pc_unresolved "cannot make a file"
# ch is a collating element of the locale: the pattern matches the name ch, not c or h, and the yesexpr of the
# locale (which takes the response ch as affirmative) is compiled in the same collating sequence
pcl_probe() {
	pcl_a=$(find fnd -type f -name '[[.ch.]]' 2>/dev/null)
	pcl_b=$(pcloc_yes find ch)
	printf '%s|%s' "$pcl_a" "$pcl_b"
}
pcl_want() {
	printf 'fnd/ch|Y'
}
pc_test
pcloc_matrix "$PCLOCY" LC_COLLATE LC_MESSAGES
pc_pass
