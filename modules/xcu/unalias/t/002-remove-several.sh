# unalias removes every name given and only those.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unalias.002
. "$PC_CORE/pc.sh"
pc_begin XCU.unalias.002
alias pcu2a='echo a' pcu2b='echo b' pcu2c='echo c'
pc_test
unalias pcu2a pcu2b
rc=$?
[ "$rc" -eq 0 ] || pc_fail "unalias exited $rc"
if alias pcu2a >/dev/null 2>&1; then pc_fail "pcu2a still defined"; fi
if alias pcu2b >/dev/null 2>&1; then pc_fail "pcu2b still defined"; fi
alias pcu2c >/dev/null 2>&1 || pc_fail "pcu2c was removed though not named"
pc_pass
