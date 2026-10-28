# Aliases reach subshells and subshell definitions stay there.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.008
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.008
pc_test
alias pcal8='echo parent'
got=$( (eval pcal8) )
[ "$got" = parent ] || pc_fail "alias not in effect in a subshell: '$got'"
( alias pcal8b='echo child' )
if alias pcal8b >/dev/null 2>&1; then
	pc_fail "alias defined in a subshell exists in the parent"
fi
pc_pass
