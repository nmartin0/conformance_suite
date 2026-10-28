# The status of case is zero when no pattern matches (whatever the status
# before it), and otherwise the status of the clause that ran.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.007
pc_test
(exit 9)
case x in a) : ;; esac
[ "$?" -eq 0 ] || pc_fail "no match gave $?"
case x in x) (exit 5) ;; esac
[ "$?" -eq 5 ] || pc_fail "match gave $?"
pc_pass
