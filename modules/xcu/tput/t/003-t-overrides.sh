# -T takes precedence over TERM
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tput.003
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tput.003
pc_pty_need
pc_pty_term
"$PC_PTY" -t 20 -o viaenv -- env "TERM=$PCT" tput clear || pc_unresolved "tput clear failed on $PCT"
pc_test
"$PC_PTY" -t 20 -o viaopt -- env "TERM=pc-no-such-terminal-type" tput -T "$PCT" clear
rc=$?
[ "$rc" -eq 0 ] || pc_fail "tput -T $PCT clear exited with status $rc when TERM named another type"
cmp -s viaenv viaopt || pc_fail "the -T type did not override TERM"
pc_pass
