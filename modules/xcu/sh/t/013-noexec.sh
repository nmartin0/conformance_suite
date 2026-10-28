# sh -n reads the commands but runs none, so a script is only syntax-
# checked: a good script gives status zero and no output; a script with a
# syntax error gives a non-zero status and a message.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.013
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.013
printf '%s\n' ': >pc.made' 'echo hi' >pc.good || pc_unresolved "write"
printf '%s\n' ': >pc.made' 'echo (' >pc.bad || pc_unresolved "write"
pc_test
r=$(${PC_SH:-sh} -n pc.good </dev/null 2>pc.err)
rc=$?
[ "$rc" -eq 0 ] && [ -z "$r" ] || pc_fail "good script: status $rc, output '$r'"
[ -e pc.made ] && pc_fail "${PC_SH:-sh} -n ran a command"
r=$(${PC_SH:-sh} -n pc.bad </dev/null 2>pc.err)
rc=$?
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "bad script: status $rc, expected 1 to 125"
[ -s pc.err ] || pc_fail "no diagnostic for the syntax error"
[ -e pc.made ] && pc_fail "${PC_SH:-sh} -n ran a command of the bad script"
pc_pass
