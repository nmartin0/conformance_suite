# A function definition stores the body without expanding it; each call
# expands and runs it afresh.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.001
pc_test
pcf() { echo "$PCV"; }
PCV=1
r=$(pcf)
[ "$r" = 1 ] || pc_fail "first call: '$r'"
PCV=2
r=$(pcf)
[ "$r" = 2 ] || pc_fail "second call: '$r'"
pch() { echo $(: >pc.def); }
[ -e pc.def ] && pc_fail "the substitution in the body ran at definition"
pch >/dev/null
[ -e pc.def ] || pc_fail "the substitution did not run at the call"
pc_pass
