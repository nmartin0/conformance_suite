# split reads standard input with no file operand or with -, and keeps an incomplete last line in the last file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.006
. "$PC_CORE/pc.sh"
pc_begin XCU.split.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb\nc' >in || pc_fail "cannot create input"
pc_test
split -l 2 <in >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split -l 2 on standard input gave status $rc"
got=$(cat xaa; printf x)
[ "$got" = "a${NL}b${NL}x" ] || pc_fail "xaa is wrong"
got=$(cat xab; printf x)
[ "$got" = "cx" ] || pc_fail "xab does not hold the incomplete last line as it was"
rm -f xaa xab
split -l 2 - pre <in >/dev/null 2>&1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split -l 2 - pre gave status $rc"
got=$(cat preaa; printf x)
[ "$got" = "a${NL}b${NL}x" ] || pc_fail "preaa is wrong"
pc_pass
