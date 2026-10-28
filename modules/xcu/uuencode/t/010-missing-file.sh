# uuencode of a nonexistent file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.010
# An unreadable or nonexistent input file gives a nonzero exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
:
pc_test
uuencode nonexistent dest.bin >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a nonexistent input file gave status 0"
pc_pass
