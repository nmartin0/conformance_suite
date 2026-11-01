# Without -r, -R or -d, rm refuses a directory, empty or not: diagnostic, non-zero status, the directory remains.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.rm.004
# MUTANT    rm.dir_removed_empty
# MUTANT    rm.dir_recursive
# MUTANT    rm.dir_no_diag
# MUTANT    rm.dir_diag_stdout
# MUTANT    rm.dir_status0
# MUTANT    rm.dir_aborts
# VARIANT   rm.status2
# VARIANT   rm.status255
# VARIANT   rm.ref
. "$PC_CORE/pc.sh"
pc_begin XCU.rm.004
mkdir d e || pc_fail "cannot create directories"
printf x >z || pc_fail "cannot create z"
printf x >e/f || pc_fail "cannot create e/f"
pc_test
rm d e z >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "rm exited 0 for directories"
[ -s err ] || pc_fail "no diagnostic"
[ -d d ] || pc_fail "empty directory d was removed"
[ -f e/f ] || pc_fail "e/f was removed"
[ ! -e z ] || pc_fail "the operand after the directories was not removed"
pc_pass
