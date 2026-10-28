# several files in order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.what.004
# Each file operand gets its own heading and strings, in operand order, and
# the status is 0 if any file matched.
. "$PC_CORE/pc.sh"
pc_begin XCU.what.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '@(#)aaa\n' >f || pc_unresolved "setup: cannot write f"
printf 'none\n' >g || pc_unresolved "setup: cannot write g"
printf '@(#)ccc\n' >h || pc_unresolved "setup: cannot write h"
pc_test
what f g h </dev/null >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "what with matches in two files: exit status $rc"
printf 'f:\n\taaa\ng:\nh:\n\tccc\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "what wrote: $(tr '\n' '|' <out)"
pc_pass
