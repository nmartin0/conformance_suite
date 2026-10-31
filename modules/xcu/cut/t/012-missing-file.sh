# A file operand that cannot be read gives a nonzero status, and later operands are still processed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.012
# MUTANT    cut.missing_status0
# MUTANT    cut.missing_abort
# MUTANT    cut.missing_no_diag
# MUTANT    cut.missing_diag_stdout
# VARIANT   cut.status2
# VARIANT   cut.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.012
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'good\n' >good || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "setup: nosuchfile exists"
pc_test
got=$(cut -c 1-2 nosuchfile good 2>err </dev/null; printf 'rc=%s' "$?")
case $got in
"go${NL}rc=0") pc_fail "status was 0 although a file could not be read" ;;
"go${NL}rc="[1-9]*) ;;
*) pc_fail "output after the missing file was not produced, or status 0" ;;
esac
[ -s err ] || pc_fail "no diagnostic was written to standard error"
pc_pass
