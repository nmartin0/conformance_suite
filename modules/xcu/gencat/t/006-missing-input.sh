# unreadable msgfile
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.gencat.006
# A msgfile that cannot be read gives a non-zero exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.gencat.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
gencat c6.cat nosuch.msg </dev/null >out 2>err
[ "$?" -gt 0 ] || pc_fail "an unreadable msgfile gave status 0"
pc_pass
