# A subshell starts as a copy of the shell: it sees all variables (exported
# or not), functions, options, working directory, mask, positional
# parameters and the value of $?.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-environment.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-environment.006
mkdir pc.sub || pc_unresolved "mkdir"
: >pc.f1
: >pc.f2
pc_test
PCV=plain
pcg() { echo fn; }
set a b
m0=$(umask)
r=$( (printf %s "$PCV") )
[ "$r" = plain ] || pc_fail "variable: '$r'"
r=$( (pcg) )
[ "$r" = fn ] || pc_fail "function: '$r'"
r=$( (echo "$#") )
[ "$r" = 2 ] || pc_fail "positional parameters: '$r'"
r=$( (umask) )
[ "$r" = "$m0" ] || pc_fail "mask: '$r'"
r=$( (pwd -P) )
[ "$r" = "$(pwd -P)" ] || pc_fail "working directory: '$r'"
false
r=$( (echo "$?") )
[ "$r" = 1 ] || pc_fail "\$? in the subshell: '$r', expected 1"
set -f
r=$( (echo pc.f*) )
set +f
[ "$r" = 'pc.f*' ] || pc_fail "option -f: '$r'"
pc_pass
