# OPTIND set to 1 starts the parse again.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.getopts.013
# MUTANT    getopts.reset_ignored
# MUTANT    getopts.reset_keeps_cluster
# VARIANT   getopts.reimpl
# VARIANT   getopts.optind_opaque
. "$PC_CORE/pc.sh"
pc_begin XCU.getopts.013
pc_test
set -- -a
getopts a pcopt
rc=$?
[ "$rc" -eq 0 ] || pc_fail "first parse exited $rc"
getopts a pcopt
rc=$?
[ "$rc" -eq 1 ] || pc_fail "end of options exited $rc"
OPTIND=1
getopts a pcopt
rc=$?
[ "$rc" -eq 0 ] && [ "$pcopt" = a ] || pc_fail "after OPTIND=1: exit $rc, name '$pcopt'"
pc_pass
