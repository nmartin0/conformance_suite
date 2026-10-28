# The commands of an AND or OR list are expanded only when they are
# executed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-lists.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-lists.004
pc_test
false && : $(: >pc.skip1)
[ -e pc.skip1 ] && pc_fail "a skipped command in && was expanded"
true || : $(: >pc.skip2)
[ -e pc.skip2 ] && pc_fail "a skipped command in || was expanded"
true && : $(: >pc.run1)
[ -e pc.run1 ] || pc_fail "an executed command in && was not expanded"
pc_pass
