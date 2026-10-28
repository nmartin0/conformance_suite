# Backslash-newline continues the line without -r only.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.005
. "$PC_CORE/pc.sh"
pc_begin XCU.read.005
printf 'abc\\\ndef\n' >in || pc_fail "cannot write the input"
pc_test
read pca <in
[ "$pca" = abcdef ] || pc_fail "without -r the variable is '$pca', wanted abcdef"
read -r pca <in
[ "$pca" = 'abc\' ] || pc_fail "with -r the variable is '$pca', wanted abc followed by a backslash"
pc_pass
