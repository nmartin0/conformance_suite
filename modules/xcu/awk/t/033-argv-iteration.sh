# ARGV processing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.033
# Elements of ARGV are processed as operands after BEGIN: a null element is
# skipped, standard input is read when no file was given, an element added in
# BEGIN is read, an assignment operand is applied when reached, and a deleted
# element is skipped.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.033
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a1\n' >fa || pc_unresolved "setup: cannot write fa"
pc_test
got=$(printf 'in\n' | awk 'BEGIN { ARGV[1] = "" } { print FILENAME ":" $0 }' pc-no-such-file 2>/dev/null)
[ "$got" = "-:in" ] || pc_fail "standard input was not read when the only operand was nulled: $got"
got=$(awk 'BEGIN { ARGV[ARGC++] = "fa" } { print FILENAME ":" $0 }' 2>/dev/null </dev/null)
[ "$got" = "fa:a1" ] || pc_fail "an element added to ARGV was not read: $got"
got=$(awk 'BEGIN { delete ARGV[1] } { print FILENAME ":" $0 }' pc-no-such-file fa 2>/dev/null </dev/null)
[ "$got" = "fa:a1" ] || pc_fail "a deleted ARGV element was not skipped: $got"
got=$(awk 'BEGIN { ARGV[ARGC++] = "v=7" } { print v ":" $0 } END { print v }' fa 2>/dev/null </dev/null)
[ "$got" = ":a1${NL}7" ] || pc_fail "an assignment element of ARGV: $got"
got=$(awk 'BEGIN { ARGC = 3 } { print FILENAME }' fa 2>/dev/null </dev/null)
[ "$got" = "fa" ] || pc_fail "an ARGC larger than the elements: $got"
pc_pass
