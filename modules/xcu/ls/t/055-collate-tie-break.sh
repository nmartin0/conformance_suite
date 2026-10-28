# Names that the collating sequence of the locale does not tell apart are ordered as in the POSIX locale.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.055
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcloc.sh"
pc_begin XCU.ls.055
LC_ALL=POSIX
export LC_ALL
pcl_d=pcl.$$
mkdir "$pcl_d" || pc_unresolved "cannot make a directory"
cd "$pcl_d" || pc_unresolved "cannot enter the directory"
trap 'cd ..; rm -rf "$pcl_d"' EXIT
pcloc_make t
mkdir lsd || pc_unresolved "cannot make a directory"
: >lsd/b || pc_unresolved "cannot make a file"
: >lsd/a || pc_unresolved "cannot make a file"
# in this locale a and b have the same weight, so the two collate as equal
pc_test
pcl_got=$(LC_ALL=$PCLOCT ls -1 lsd 2>/dev/null)
[ "$pcl_got" = "a$pcloc_nl"b ] || pc_fail "ls with a and b collating as equal gave [$pcl_got], not a then b (byte order)"
pc_pass
