# When no readable file is found, an interactive shell writes a diagnostic
# to standard error, does not exit, and the dot utility returns a non-zero
# status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dot.006
. "$PC_CORE/pc.sh"
pc_begin XCU.dot.006
# an interactive shell may keep a history file; keep it in the working directory
HISTFILE=$PWD/pc.hist
export HISTFILE
printf '%s\n' 'exit 0' >pc.i || pc_unresolved "write"
${PC_SH:-sh} -i <pc.i >/dev/null 2>&1 || pc_unresolved "cannot start an interactive shell with ${PC_SH:-sh} -i"
printf '%s\n' '. ./pc.nosuchfile' 'echo "st=$?"' 'echo after' >pc.s || pc_unresolved "write"
pc_test
out=$(${PC_SH:-sh} -i <pc.s 2>pc.err)
case $out in
*after*) ;;
*) pc_fail "interactive shell did not continue (out='$out')" ;;
esac
case $out in
*st=0*) pc_fail "dot returned zero for a missing file (out='$out')" ;;
*st=[1-9]*) ;;
*) pc_fail "no status line in '$out'" ;;
esac
[ -s pc.err ] || pc_fail "no diagnostic on standard error"
pc_pass
