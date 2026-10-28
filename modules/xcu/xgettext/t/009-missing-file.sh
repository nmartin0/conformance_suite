# unreadable input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xgettext.009
# A file operand that cannot be read gives a non-zero exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.xgettext.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
xgettext nosuchfile.c </dev/null >o 2>e
[ "$?" -gt 0 ] || pc_fail "an unreadable input file gave status 0"
pc_pass
