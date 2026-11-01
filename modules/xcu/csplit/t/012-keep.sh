# csplit -k is accepted; without an error it gives the same files and sizes, and an error still gives a nonzero status and a diagnostic.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.csplit.012
# MUTANT    csplit.k_rejected
# MUTANT    csplit.k_no_sizes
# MUTANT    csplit.k_no_files
# MUTANT    csplit.k_status1
# MUTANT    csplit.k_error_status0
# MUTANT    csplit.k_error_no_diag
# VARIANT   csplit.err_prevalidate
. "$PC_CORE/pc.sh"
pc_begin XCU.csplit.012
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 6 7 8 9 10 >in10 || pc_fail "cannot create input"
pc_test
got=$(csplit -k in10 3 6 2>err </dev/null; printf 'rc=%s' "$?")
[ "$got" = "4${NL}6${NL}11${NL}rc=0" ] || pc_fail "-k: the size lines or the status are wrong"
[ -s err ] && pc_fail "-k with no error wrote to standard error"
got=$(cat xx00; printf x)
[ "$got" = "1${NL}2${NL}x" ] || pc_fail "-k: xx00 is wrong"
got=$(cat xx01; printf x)
[ "$got" = "3${NL}4${NL}5${NL}x" ] || pc_fail "-k: xx01 is wrong"
got=$(cat xx02; printf x)
[ "$got" = "6${NL}7${NL}8${NL}9${NL}10${NL}x" ] || pc_fail "-k: xx02 is wrong"
rm -f xx00 xx01 xx02
csplit -ks in10 3 6 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-ks: status $rc"
[ -s out ] && pc_fail "-ks wrote to standard output"
[ -s xx00 ] && [ -s xx01 ] && [ -s xx02 ] || pc_fail "-ks: the pieces are missing"
rm -f xx00 xx01 xx02
# the files that remain after an error are not examined: an implementation
# may find the error before it makes a file
csplit -k in10 3 '/nomatchatall/' >/dev/null 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-k: status 0 although the expression matched no line"
[ -s err ] || pc_fail "-k: no diagnostic on standard error"
pc_pass
