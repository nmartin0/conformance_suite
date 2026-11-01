# alias for a missing name writes a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.006
# MUTANT    alias.missing_nodiag
# MUTANT    alias.missing_stdout_diag
# MUTANT    alias.missing_nodiag_multi
#
# The wording is not fixed, so only its presence is checked, also when the
# missing name is one of several.
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.006
pc_test
alias pcal6='echo six'
alias pcnosuch2 >/dev/null 2>err
[ -s err ] || pc_fail "no diagnostic on standard error"
alias pcal6 pcnosuch2 >/dev/null 2>err2
[ -s err2 ] || pc_fail "no diagnostic when the missing name follows an existing one"
alias pcnosuch2 pcal6 >/dev/null 2>err3
[ -s err3 ] || pc_fail "no diagnostic when the missing name precedes an existing one"
pc_pass
