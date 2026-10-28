# sh -v and sh -x act as set -v and set -x: the input lines, or a trace of
# each expanded command, go to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.014
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.014
printf '%s\n' 'pcv=expanded_val' 'echo "$pcv" marker_line' >pc.cf || pc_unresolved "write"
pc_test
${PC_SH:-sh} -v pc.cf </dev/null >pc.o 2>pc.err
grep 'echo "\$pcv" marker_line' pc.err >/dev/null || pc_fail "-v: input line not on standard error"
[ "$(cat pc.o)" = "expanded_val marker_line" ] || pc_fail "-v: standard output was '$(cat pc.o)'"
${PC_SH:-sh} -x pc.cf </dev/null >pc.o 2>pc.err
grep 'expanded_val marker_line' pc.err >/dev/null || pc_fail "-x: no trace of the expanded command on standard error"
pc_pass
