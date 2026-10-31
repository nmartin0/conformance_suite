# uniq reads input_file (or standard input for no operand or -) and writes output_file when one is given, leaving standard output empty.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uniq.007
# MUTANT    uniq.out_ignored
# MUTANT    uniq.out_also_stdout
# MUTANT    uniq.out_append
# MUTANT    uniq.dash_literal
# MUTANT    uniq.swap_operands
# MUTANT    uniq.stdin_always
# MUTANT    uniq.missing_status0
# MUTANT    uniq.missing_no_diag
# MUTANT    uniq.missing_diag_stdout
# MUTANT    uniq.outerr_status0
# MUTANT    uniq.outerr_no_diag
# MUTANT    uniq.status1
# VARIANT   uniq.status2
# VARIANT   uniq.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.uniq.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\na\nb\n' >in || pc_fail "cannot create input"
printf 'old\nolder\noldest\nstill older than the rest\n' >res || pc_fail "cannot create output file"
printf 'other\n' >other || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "setup: nosuchfile exists"
[ ! -e nodir ] || pc_unresolved "setup: nodir exists"
pc_test
uniq in res >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "uniq in res gave status $rc"
[ -s out ] && pc_fail "standard output was written although output_file was given"
[ -f res ] || pc_fail "output_file was not made"
got=$(cat res; printf x)
[ "$got" = "a${NL}b${NL}x" ] || pc_fail "output_file does not hold the result"
uniq - res2 <in >out 2>/dev/null
rc=$?
[ -f res2 ] || pc_fail "output_file was not made for input -"
[ "$rc" -eq 0 ] || pc_fail "uniq - res2 gave status $rc"
[ -s out ] && pc_fail "standard output was written although output_file was given with -"
got=$(cat res2; printf x)
[ "$got" = "a${NL}b${NL}x" ] || pc_fail "uniq - res2 did not read standard input"
uniq in <other >out 2>/dev/null
[ "$(cat out)" = "a${NL}b" ] || pc_fail "standard input was read although input_file was given"
uniq nosuchfile >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "status was 0 although input_file could not be read"
[ -s out ] && pc_fail "standard output was written for an unreadable input_file"
[ -s err ] || pc_fail "no diagnostic was written to standard error for an unreadable input_file"
uniq in nodir/res3 >out 2>err </dev/null
rc=$?
[ "$rc" -gt 0 ] || pc_fail "status was 0 although output_file could not be written"
[ -s err ] || pc_fail "no diagnostic was written to standard error for an unwritable output_file"
pc_pass
