# eval joins its operands with single spaces into one command string and
# executes it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.eval.001
. "$PC_CORE/pc.sh"
pc_begin XCU.eval.001
pc_test
r=$(eval echo a b)
[ "$r" = "a b" ] || pc_fail "eval echo a b printed '$r'"
r=$(eval echo 'one   two')
[ "$r" = "one two" ] || pc_fail "eval echo 'one   two' printed '$r'"
eval 'pcx=1' 'pcy=2'
[ "$pcx$pcy" = 12 ] || pc_fail "two assignment operands: pcx='$pcx' pcy='$pcy'"
pc_pass
