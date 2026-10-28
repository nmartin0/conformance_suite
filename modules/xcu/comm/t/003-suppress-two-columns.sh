# comm with two of -1, -2, -3 writes the remaining column with no leading tab, and with all three writes nothing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.comm.003
. "$PC_CORE/pc.sh"
pc_begin XCU.comm.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\nc\nd\n' >f1 && printf 'b\nc\ne\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(comm -12 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}c${NL}rc=0" ] || pc_fail "-12 did not write just the common lines"
got=$(comm -13 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "e${NL}rc=0" ] || pc_fail "-13 did not write just the lines only in file2"
got=$(comm -23 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}d${NL}rc=0" ] || pc_fail "-23 did not write just the lines only in file1"
got=$(comm -1 -2 -3 f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-1 -2 -3 did not write nothing"
pc_pass
