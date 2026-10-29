# unalias of a missing name: no output, a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unalias.005
. "$PC_CORE/pc.sh"
pc_begin XCU.unalias.005
pc_test
alias pcu5=x
unalias pcu5 >/dev/null 2>&1 || pc_unresolved "unalias of a defined alias fails, so a diagnostic for a missing name proves nothing"
unalias pcnosuch4 >out 2>err
[ ! -s out ] || pc_fail "unalias wrote to standard output"
[ -s err ] || pc_fail "no diagnostic on standard error"
pc_pass
