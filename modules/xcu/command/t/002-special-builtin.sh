# command removes the special property of a special built-in.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.002
# MUTANT    command.special_kept
#
# Three special built-ins; for each, an assignment written before command
# must not stay in the shell afterwards.
. "$PC_CORE/pc.sh"
pc_begin XCU.command.002
unset pcv pcw pcx pcy
pc_test
pcv=set command : 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command : exited $rc"
[ "${pcv+x}" != x ] || pc_fail "an assignment before command : stayed in the shell"
pcw=set command eval : 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command eval : exited $rc"
[ "${pcw+x}" != x ] || pc_fail "an assignment before command eval stayed in the shell"
pcx=set command export pcy=1 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command export exited $rc"
[ "${pcx+x}" != x ] || pc_fail "an assignment before command export stayed in the shell"
pc_pass
