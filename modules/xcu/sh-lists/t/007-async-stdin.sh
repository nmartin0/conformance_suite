# When job control is off, an asynchronous list gets /dev/null as standard
# input unless it redirects it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-lists.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-lists.007
printf 'data\n' >pc.in2 || pc_unresolved "write"
printf '%s\n' 'cat >pc.as1 &' 'wait' 'cat <pc.in2 >pc.as2 &' 'wait' >pc.s || pc_unresolved "write"
pc_test
printf 'fromstdin\n' | ${PC_SH:-sh} pc.s >/dev/null 2>&1
[ -f pc.as1 ] && [ ! -s pc.as1 ] || pc_fail "the asynchronous cat read the shell's standard input"
[ "$(cat pc.as2)" = data ] || pc_fail "an explicit redirection was not honoured"
pc_pass
