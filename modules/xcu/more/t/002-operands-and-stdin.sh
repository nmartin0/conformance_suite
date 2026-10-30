# several files, standard input, and the - operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.more.002
# With no file operands standard input is copied; with several operands they
# are copied in order one after another; a - operand reads standard input at
# that point in the sequence.
. "$PC_CORE/pc.sh"
pc_begin XCU.more.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'f1a\nf1b\n' >f1
printf 'f2a\n' >f2
printf 'sa\nsb\nsc\n' >sin
pc_test
more <sin >o1 2>err || pc_fail "more from standard input: nonzero status: $(sed 1q err)"
cmp sin o1 >/dev/null 2>&1 || pc_fail "standard input was not copied"
more f1 f2 </dev/null >o2 2>err || pc_fail "more f1 f2: nonzero status"
printf 'f1a\nf1b\nf2a\n' >e2
cmp e2 o2 >/dev/null 2>&1 || pc_fail "two files not copied in order: $(cat o2)"
more f1 - f2 <sin >o3 2>err || pc_fail "more f1 - f2: nonzero status"
printf 'f1a\nf1b\nsa\nsb\nsc\nf2a\n' >e3
cmp e3 o3 >/dev/null 2>&1 || pc_fail "- did not read standard input in sequence: $(cat o3)"
pc_pass
