# -s squeezes consecutive empty lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.more.003
# With -s runs of consecutive empty lines in the input come out as a single
# empty line and everything else is unchanged.
. "$PC_CORE/pc.sh"
pc_begin XCU.more.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a\n\n\n\nb\n\nc\n\n\nd\n' >in
printf 'a\n\nb\n\nc\n\nd\n' >want
pc_test
more -s in </dev/null >out 2>err || pc_fail "more -s: nonzero status: $(sed 1q err)"
cmp want out >/dev/null 2>&1 || pc_fail "-s output wrong: $(cat out)"
more in </dev/null >plain 2>err || pc_fail "more: nonzero status: $(sed 1q err)"
cmp in plain >/dev/null 2>&1 || pc_fail "without -s the empty lines were not kept"
pc_pass
