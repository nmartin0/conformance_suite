# sh -i makes the shell interactive, so "$-" contains i; a shell started
# without -i, with no terminal, is not interactive.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.016
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.016
# an interactive shell may keep a history file; keep it in the working directory
HISTFILE=$PWD/pc.hist
export HISTFILE
printf '%s\n' 'exit 0' >pc.i || pc_unresolved "write"
${PC_SH:-sh} -i <pc.i >/dev/null 2>&1 || pc_unresolved "cannot start an interactive shell with ${PC_SH:-sh} -i"
printf '%s\n' 'case $- in *i*) echo yes ;; *) echo no ;; esac' >pc.s || pc_unresolved "write"
pc_test
r=$(${PC_SH:-sh} -i <pc.s 2>/dev/null)
[ "$r" = yes ] || pc_fail "${PC_SH:-sh} -i: \$- has no i (got '$r')"
r=$(${PC_SH:-sh} <pc.s 2>/dev/null)
[ "$r" = no ] || pc_fail "${PC_SH:-sh} without -i from a file: \$- has i (got '$r')"
r=$(${PC_SH:-sh} -c 'case $- in *i*) echo yes ;; *) echo no ;; esac' </dev/null)
[ "$r" = no ] || pc_fail "${PC_SH:-sh} -c: \$- has i (got '$r')"
pc_pass
