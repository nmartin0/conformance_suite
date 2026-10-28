# The operands are expanded once by the shell and the result is parsed
# again, so eval y='$'$x with x holding a variable name gives y that
# variable's value.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.eval.003
. "$PC_CORE/pc.sh"
pc_begin XCU.eval.003
pc_test
foo=10 x=foo
y='$'$x
[ "$y" = '$foo' ] || pc_fail "plain assignment gave '$y'"
eval y='$'$x
[ "$y" = 10 ] || pc_fail "eval assignment gave '$y', expected 10"
pc_pass
