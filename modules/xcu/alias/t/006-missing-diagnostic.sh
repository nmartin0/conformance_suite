# alias for a missing name writes a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.006
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.006
pc_test
alias pcnosuch2 >/dev/null 2>err
[ -s err ] || pc_fail "no diagnostic on standard error"
pc_pass
