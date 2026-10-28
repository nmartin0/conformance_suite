# A pattern that matches no name is left as it is; a word with no pattern
# characters is also left alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.007
mkdir pc.fe || pc_unresolved "mkdir"
: >pc.fe/a
pc_test
r=$(cd pc.fe && echo nomatch*)
[ "$r" = 'nomatch*' ] || pc_fail "got '$r' for nomatch*"
r=$(cd pc.fe && echo zz?)
[ "$r" = 'zz?' ] || pc_fail "got '$r' for zz?"
r=$(cd pc.fe && echo [x]y)
[ "$r" = '[x]y' ] || pc_fail "got '$r' for [x]y"
r=$(cd pc.fe && echo plain)
[ "$r" = plain ] || pc_fail "got '$r' for plain"
pc_pass
