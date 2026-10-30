# MORE holds options placed before the command line ones
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.more.004
# Options in the MORE variable act as if written before the command line
# options, so MORE=-s squeezes empty lines in the same way as -s.
. "$PC_CORE/pc.sh"
pc_begin XCU.more.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a\n\n\n\nb\n' >in
printf 'a\n\nb\n' >want
pc_test
MORE=-s; export MORE
more in </dev/null >out 2>err || pc_fail "more with MORE=-s: nonzero status: $(sed 1q err)"
cmp want out >/dev/null 2>&1 || pc_fail "MORE=-s did not squeeze: $(cat out)"
pc_pass
