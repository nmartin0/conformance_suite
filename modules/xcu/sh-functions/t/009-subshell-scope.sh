# A function defined in a subshell is not defined in the shell that started
# it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.009
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.009
pc_test
( pcz() { :; } )
(pcz) 2>/dev/null
[ "$?" -eq 127 ] || pc_fail "the function leaked out of the subshell (status $?)"
pc_pass
