# -T takes precedence over TERM
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tabs.005
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.tabs.005
pc_pty_need
pc_pty_term
TERM=$PCT "$PC_PTY" -t 20 -o viaenv -- tabs -8 || pc_unresolved "tabs -8 is not possible on this terminal type"
pc_test
TERM=pc-no-such-terminal-type "$PC_PTY" -t 20 -o viaopt -- tabs -T "$PCT" -8 || pc_fail "tabs -T $PCT failed when TERM named another type"
[ -s viaenv ] || pc_unresolved "tabs wrote nothing to the terminal, so the effect of -T cannot be compared"
cmp -s viaenv viaopt || pc_fail "the -T type did not override TERM"
pc_pass
