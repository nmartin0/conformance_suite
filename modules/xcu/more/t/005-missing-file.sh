# file operand that cannot be read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.more.005
# A file operand that cannot be read gives a non-zero exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.more.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'x\n' >ok
pc_test
more nosuch </dev/null >out 2>err && pc_fail "more of a missing file exited 0"
pc_pass
