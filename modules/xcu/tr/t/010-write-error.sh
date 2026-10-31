# When tr cannot write its output it gives a diagnostic on standard error and a nonzero status.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.010
# MUTANT    tr.write_status0
# MUTANT    tr.write_no_diag
# VARIANT   tr.status2
# VARIANT   tr.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.010
LC_ALL=POSIX
export LC_ALL
pc_test
# the standard output is closed, so the write must fail
{ printf 'abc\n' | tr a b >&-; echo "$?" >rc; } 2>err
[ -f rc ] || pc_unresolved "cannot record the exit status"
read -r rc <rc
case $rc in
0) pc_fail "tr exited 0 although it could not write its output" ;;
esac
[ -s err ] || pc_fail "no diagnostic was written to standard error"
pc_pass
