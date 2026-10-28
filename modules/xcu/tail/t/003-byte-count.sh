# tail -c number starts relative to the end of the file in bytes, or from the numberth byte when the number has a plus sign.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.003
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij\n' >in || pc_fail "cannot create input"
pc_test
got=$(tail -c 4 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "hij${NL}rc=0" ] || pc_fail "-c 4 did not write the last four bytes"
got=$(tail -c -4 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "hij${NL}rc=0" ] || pc_fail "-c -4 did not write the last four bytes"
got=$(tail -c +9 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ij${NL}rc=0" ] || pc_fail "-c +9 did not start at the ninth byte"
got=$(tail -c +1 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}rc=0" ] || pc_fail "-c +1 did not write the whole file"
got=$(tail -c 1 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}rc=0" ] || pc_fail "-c 1 did not write the last byte"
pc_pass
