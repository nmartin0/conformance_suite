# read takes one line from a seekable input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.read.011
. "$PC_CORE/pc.sh"
pc_begin XCU.read.011
nl='
'
printf 'l1\nl2\nl3\n' >in || pc_fail "cannot write the input"
pc_test
{
	read pca
	cat
} <in >rest
[ "$pca" = l1 ] || pc_fail "read gave '$pca', wanted l1"
got=$(cat rest; echo x)
[ "$got" = "l2${nl}l3${nl}x" ] || pc_fail "the rest of the file after read was not l2 and l3"
pc_pass
