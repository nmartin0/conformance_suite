# An interactive shell does not exit for a syntax error, a special built-in
# error, a redirection error of a special built-in, a variable assignment
# error, an expansion error, or an unknown command; later commands still
# run.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-errors.010
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-errors.010
# an interactive shell may keep a history file; keep it in the working directory
HISTFILE=$PWD/pc.hist
export HISTFILE
printf '%s\n' 'exit 0' >pc.i || pc_unresolved "write"
${PC_SH:-sh} -i <pc.i >/dev/null 2>&1 || pc_unresolved "cannot start an interactive shell with ${PC_SH:-sh} -i"
pc_test
for cmd in 'echo (' 'set -Z' ': <pc.nosuchfile' 'readonly PCR=1; PCR=2' 'echo ${PCU1?msg}'; do
	printf '%s\n' "$cmd" 'echo after' >pc.s || pc_unresolved "write"
	out=$(${PC_SH:-sh} -i <pc.s 2>/dev/null)
	case $out in
	*after*) ;;
	*) pc_fail "$cmd: the interactive shell did not go on (out='$out')" ;;
	esac
done
printf '%s\n' 'pc-no-such-utility-xyz' 'echo "after $?"' >pc.s || pc_unresolved "write"
out=$(${PC_SH:-sh} -i <pc.s 2>/dev/null)
case $out in
*"after 127"*) ;;
*) pc_fail "unknown command: out='$out', expected after 127" ;;
esac
pc_pass
