# uudecode of a nonexistent input file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.009
# A nonexistent input file gives a nonzero exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
:
pc_test
uudecode nonexistent >stdout 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a nonexistent input file gave status 0"
pc_pass
