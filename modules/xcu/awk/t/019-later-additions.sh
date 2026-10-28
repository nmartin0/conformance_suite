# nextfile skips the rest of the current file, fflush flushes output and returns 0, and FILENAME is - while reading standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.019
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.019
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a1\na2\na3\n' >fa && printf 'b1\nb2\n' >fb || pc_fail "cannot create input"
pc_test
got=$(awk 'FNR == 2 { nextfile } { print FILENAME ":" $0 }' fa fb 2>/dev/null </dev/null)
[ "$got" = "fa:a1${NL}fb:b1" ] || pc_fail "nextfile did not skip the rest of each file"
got=$(awk 'FNR == 2 { nextfile } END { print NR }' fa fb 2>/dev/null </dev/null)
[ "$got" = "4" ] || pc_fail "NR was wrong after nextfile or END did not run"
got=$(awk 'BEGIN { print "x"; r = fflush(); print r }' 2>/dev/null </dev/null)
[ "$got" = "x${NL}0" ] || pc_fail "fflush did not return 0"
got=$(awk 'BEGIN { print "x" > "ff"; r = fflush("ff"); print r; close("ff") }' 2>/dev/null </dev/null)
[ "$got" = "0" ] || pc_fail "fflush of a named file did not return 0"
[ "$(cat ff 2>/dev/null)" = "x" ] || pc_fail "the file named to fflush was not written"
got=$(printf 'z\n' | awk '{ print FILENAME }' 2>/dev/null)
[ "$got" = "-" ] || pc_fail "FILENAME was not - for standard input"
got=$(printf 'z\n' | awk '{ print FILENAME ":" $0 }' - 2>/dev/null)
[ "$got" = "-:z" ] || pc_fail "FILENAME was not - for an operand -"
pc_pass
