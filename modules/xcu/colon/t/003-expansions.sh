# The operands of the null utility are still expanded: an assignment made by
# an expansion in an operand and a command substitution in an operand both
# take effect.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.colon.003
. "$PC_CORE/pc.sh"
pc_begin XCU.colon.003
unset pcx pcy
pc_test
: "${pcx=abc}" $((pcy=7)) "$(printf ran >pc.sub)"
[ "$pcx" = abc ] || pc_fail "${pcx=abc} did not assign (pcx='$pcx')"
[ "$pcy" = 7 ] || pc_fail "arithmetic assignment did not take effect (pcy='$pcy')"
[ -f pc.sub ] || pc_fail "command substitution operand was not run"
pc_pass
