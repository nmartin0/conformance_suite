# Redirections of a command with no command name are done in a subshell: an
# output redirection creates or truncates the file, and a descriptor opened
# that way is not open afterwards.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.009
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.009
printf old >pc.n1
{ : >&7; } 2>/dev/null && pc_unresolved "descriptor 7 is already open"
pc_test
>pc.n1
[ -f pc.n1 ] && [ ! -s pc.n1 ] || pc_fail "the file was not truncated"
7>pc.n2
{ echo hi >&7; } 2>/dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "descriptor 7 stayed open after a command with no name"
pc_pass
