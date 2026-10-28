# sort -c and -C check the order without writing output: status 0 if ordered, 1 if not (or if -u finds a duplicate); -c writes a diagnostic, -C does not.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sort.010
. "$PC_CORE/pc.sh"
pc_begin XCU.sort.010
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\nc\n' >ok || pc_fail "cannot create input"
printf 'a\nc\nb\n' >bad || pc_fail "cannot create input"
printf 'a\na\nb\n' >dup || pc_fail "cannot create input"
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
pc_pass
