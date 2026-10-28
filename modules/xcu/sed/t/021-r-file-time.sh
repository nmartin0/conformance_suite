# r reads its file when the output is written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.021
# The contents of the file named by r are those at the time the output is
# written, not at the time the r command ran, so lines a w command wrote to
# that file in the same cycle are included.
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.021
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'L1\n' >in || pc_fail "cannot create input"
pc_test
got=$(sed -e 'r rf' -e 'w rf' in 2>/dev/null </dev/null)
[ "$got" = "L1${NL}L1" ] || pc_fail "r did not read the file as of the time of output: $got"
pc_pass
