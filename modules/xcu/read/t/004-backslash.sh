# Backslash escapes without -r, is kept with -r.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.004
. "$PC_CORE/pc.sh"
pc_begin XCU.read.004
printf 'a\\ b c\n' >in1 || pc_fail "cannot write the input"
printf 'x\\\\y\n' >in2 || pc_fail "cannot write the input"
pc_test
read pca pcb <in1
[ "$pca" = 'a b' ] || pc_fail "without -r the first variable is '$pca', wanted 'a b'"
[ "$pcb" = c ] || pc_fail "without -r the second variable is '$pcb', wanted c"
read -r pca pcb <in1
[ "$pca" = 'a\' ] || pc_fail "with -r the first variable is '$pca', wanted a followed by a backslash"
[ "$pcb" = 'b c' ] || pc_fail "with -r the second variable is '$pcb', wanted 'b c'"
read pca <in2
[ "$pca" = 'x\y' ] || pc_fail "without -r a doubled backslash gives '$pca', wanted x\\y"
read -r pca <in2
[ "$pca" = 'x\\y' ] || pc_fail "with -r a doubled backslash gives '$pca', wanted x\\\\y"
pc_pass
