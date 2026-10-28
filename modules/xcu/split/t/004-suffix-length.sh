# split -a n gives the file names suffixes of n lowercase letters, counting up from all a.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.004
. "$PC_CORE/pc.sh"
pc_begin XCU.split.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '%s\n' 1 2 3 4 5 >in || pc_fail "cannot create input"
pc_test
split -a 3 -l 2 in >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split -a 3 gave status $rc"
got=$(cat xaaa; printf x)
[ "$got" = "1${NL}2${NL}x" ] || pc_fail "xaaa is wrong"
got=$(cat xaab; printf x)
[ "$got" = "3${NL}4${NL}x" ] || pc_fail "xaab is wrong"
got=$(cat xaac; printf x)
[ "$got" = "5${NL}x" ] || pc_fail "xaac is wrong"
[ -e xaa ] && pc_fail "a file with a two-letter suffix was created"
split -a 1 -l 2 in s >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split -a 1 gave status $rc"
got=$(cat sa; printf x)
[ "$got" = "1${NL}2${NL}x" ] || pc_fail "sa is wrong"
got=$(cat sc; printf x)
[ "$got" = "5${NL}x" ] || pc_fail "sc is wrong"
pc_pass
