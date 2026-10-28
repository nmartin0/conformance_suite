# c17 compiles a .i operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.c17.009
# The output of "c17 -E x.c" saved as x.i compiles and links with "c17 -o p
# x.i" into a program with the original behaviour.
. "$PC_CORE/pc.sh"
pc_begin XCU.c17.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '#include <stdio.h>\nint main(void) { puts("ei"); return 4; }\n' >x.c
c17 -E x.c >x.i 2>/dev/null </dev/null || pc_unresolved "setup: c17 -E failed"
pc_test
c17 -o p x.i >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "c17 x.i: exit status $rc"
./p >o </dev/null
rc=$?
[ "$rc" -eq 4 ] || pc_fail "the program exits with $rc, expected 4"
[ "$(cat o)" = ei ] || pc_fail "the program printed '$(cat o)'"
pc_pass
