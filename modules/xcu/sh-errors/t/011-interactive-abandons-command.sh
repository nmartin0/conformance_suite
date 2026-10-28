# After an error that an interactive shell survives, it does not run the
# rest of the command in which the error happened.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.011
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.011
# an interactive shell may keep a history file; keep it in the working directory
HISTFILE=$PWD/pc.hist
export HISTFILE
printf '%s\n' 'exit 0' >pc.i || pc_unresolved "write"
${PC_SH:-sh} -i <pc.i >/dev/null 2>&1 || pc_unresolved "cannot start an interactive shell with ${PC_SH:-sh} -i"
printf '%s\n' 'if true; then set -Z; echo two; fi' 'echo after' >pc.s || pc_unresolved "write"
pc_test
out=$(${PC_SH:-sh} -i <pc.s 2>/dev/null)
case $out in
*two*) pc_fail "the rest of the if command ran: '$out'" ;;
esac
case $out in
*after*) ;;
*) pc_fail "the shell did not go on: '$out'" ;;
esac
pc_pass
