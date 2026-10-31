# sort -c and -C check the order without writing output: status 0 if ordered, 1 if not (or if -u finds a duplicate); -c writes a diagnostic, -C does not.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.010
# MUTANT    sort.c_no_diag
# MUTANT    sort.C_diag
# MUTANT    sort.c_status0
# MUTANT    sort.c_status2
# MUTANT    sort.c_prints
# MUTANT    sort.u_dup_ok
# MUTANT    sort.c_ok_status1
# MUTANT    sort.c_skips_first
# MUTANT    sort.c_misses_last
# MUTANT    sort.c_ignores_opts
# VARIANT   sort.diag_lineno
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\nc\n' >ok || pc_fail "cannot create input"
printf 'a\nc\nb\n' >bad || pc_fail "cannot create input"
printf 'a\na\nb\n' >dup || pc_fail "cannot create input"
printf 'b\na\nc\n' >bad2 || pc_fail "cannot create input"
printf 'c\nb\na\n' >desc || pc_fail "cannot create input"
pc_test
sort -c ok >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-c on ordered input gave status $rc"
[ -s out ] && pc_fail "-c wrote to standard output"
sort -c bad >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-c on unordered input gave status $rc, not 1"
[ -s out ] && pc_fail "-c wrote to standard output for unordered input"
[ -s err ] || pc_fail "-c wrote no diagnostic for unordered input"
sort -C bad >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-C on unordered input gave status $rc, not 1"
[ -s out ] && pc_fail "-C wrote to standard output"
[ -s err ] && pc_fail "-C wrote a diagnostic"
sort -C ok >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-C on ordered input gave status $rc"
sort -c -u dup >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-c -u with a duplicate gave status $rc, not 1"
sort -c -u ok >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-c -u on ordered unique input gave status $rc"
sort -c bad2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-c with disorder at the first pair gave status $rc, not 1"
sort -c -r desc >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-c -r on input in descending order gave status $rc"
sort -c -r ok >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-c -r on input in ascending order gave status $rc, not 1"
sort -C -u dup >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-C -u with a duplicate gave status $rc, not 1"
[ -s out ] && pc_fail "-C -u wrote to standard output"
[ -s err ] && pc_fail "-C -u wrote a diagnostic"
sort -c -u dup >out 2>err </dev/null
[ -s out ] && pc_fail "-c -u wrote to standard output"
[ -s err ] || pc_fail "-c -u wrote no diagnostic for a duplicate key"
pc_pass
