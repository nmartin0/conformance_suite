# Lines that the collating sequence of the locale does not tell apart are ordered as in the POSIX locale.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.020
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.sort.020
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make t
# in this locale a and b have the same weight, so the two collate as equal
pc_test
for pcl_in in 'b\na\n' 'a\nb\n'
do
	pcl_got=$(printf "$pcl_in" | LC_ALL=$PCLOCT sort 2>/dev/null)
	[ "$pcl_got" = "a$pcloc_nl"b ] || pc_fail "sort of [$pcl_in] with a and b collating as equal gave [$pcl_got], not a then b (byte order)"
done
pc_pass
