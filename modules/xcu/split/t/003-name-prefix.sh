# A name operand replaces x as the file name prefix.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.003
. "$PC_CORE/pc.sh"
pc_begin XCU.split.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 >in || pc_fail "cannot create input"
pc_test
split -l 2 in out >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split -l 2 in out gave status $rc"
got=$(cat outaa; printf x)
[ "$got" = "1${NL}2${NL}x" ] || pc_fail "outaa is wrong"
got=$(cat outab; printf x)
[ "$got" = "3${NL}4${NL}x" ] || pc_fail "outab is wrong"
[ -e xaa ] && pc_fail "a file with the default prefix was created"
pc_pass
