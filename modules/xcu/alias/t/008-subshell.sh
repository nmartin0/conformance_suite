# Aliases reach subshells and subshell definitions stay there.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.008
# MUTANT    alias.sub_no_inherit
# MUTANT    alias.sub_leaks
# VARIANT   alias.missing_status2
# VARIANT   alias.missing_status255
#
# The subshells are a parenthesised list, a command substitution and a
# member of a pipeline. The alias is used by eval inside them.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.008
# use TEXT: run TEXT through the shell, so that its aliases are looked up
use() { eval "$@"; }
pc_test
alias pcal8='echo parent'
(use pcal8) >o1 2>/dev/null
got=$(cat o1)
[ "$got" = parent ] || pc_fail "alias not in effect in a subshell: '$got'"
got=$( (use pcal8) 2>/dev/null )
[ "$got" = parent ] || pc_fail "alias not in effect in a nested subshell: '$got'"
got=$(use pcal8 2>/dev/null)
[ "$got" = parent ] || pc_fail "alias not in effect in a command substitution: '$got'"
{ use pcal8; } 2>/dev/null | cat >o2
got=$(cat o2)
[ "$got" = parent ] || pc_fail "alias not in effect in a pipeline member: '$got'"
# a definition in a subshell is in effect there
( alias pcal8='echo child'; use pcal8 >o3 2>/dev/null )
got=$(cat o3)
[ "$got" = child ] || pc_fail "redefinition in a subshell not in effect there: '$got'"
# and does not reach the shell that started the subshell
use pcal8 >o4 2>/dev/null
got=$(cat o4)
[ "$got" = parent ] || pc_fail "redefinition in a subshell changed the parent: '$got'"
( alias pcal8b='echo child' )
if alias pcal8b >/dev/null 2>&1; then
	pc_fail "alias defined in a subshell exists in the parent"
fi
use pcal8b >o5 2>/dev/null
got=$(cat o5)
[ "$got" != child ] || pc_fail "alias defined in a subshell is substituted in the parent"
pc_pass
