# unalias -a removes all aliases.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unalias.003
. "$PC_CORE/pc.sh"
pc_begin XCU.unalias.003
alias pcu3a='echo a' pcu3b='echo b'
pc_test
unalias -a
rc=$?
[ "$rc" -eq 0 ] || pc_fail "unalias -a exited $rc"
alias >out 2>/dev/null
[ ! -s out ] || pc_fail "aliases remain after unalias -a"
if alias pcu3a >/dev/null 2>&1; then pc_fail "pcu3a still defined"; fi
pc_pass
