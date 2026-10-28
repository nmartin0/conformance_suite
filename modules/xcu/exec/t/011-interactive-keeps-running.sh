# A failing exec does not make an interactive shell exit (except from a
# subshell, where it may).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.exec.011
. "$PC_CORE/pc.sh"
pc_begin XCU.exec.011
# an interactive shell may keep a history file; keep it in the working directory
HISTFILE=$PWD/pc.hist
export HISTFILE
printf '%s\n' 'exit 0' >pc.i || pc_unresolved "write"
${PC_SH:-sh} -i <pc.i >/dev/null 2>&1 || pc_unresolved "cannot start an interactive shell with ${PC_SH:-sh} -i"
printf '%s\n' 'exec pc-no-such-utility-xyz' 'echo after' >pc.s || pc_unresolved "write"
pc_test
out=$(${PC_SH:-sh} -i <pc.s 2>pc.err)
case $out in
*after*) ;;
*) pc_fail "interactive shell exited after the failed exec (out='$out')" ;;
esac
pc_pass
