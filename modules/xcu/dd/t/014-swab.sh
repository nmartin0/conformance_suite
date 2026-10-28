# dd conv=swab
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.014
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Pairs of input bytes are swapped; an odd last byte stays.
printf 'abcdef' >f6 || pc_fail "cannot write f6"
printf 'abcde' >f5 || pc_fail "cannot write f5"
printf 'abcdefg' >f7 || pc_fail "cannot write f7"
printf 'badcfe' >e6 || pc_fail "cannot write e6"
printf 'badce' >e5 || pc_fail "cannot write e5"
printf 'badcfeg' >e7 || pc_fail "cannot write e7"
pc_test
chk() {
	dd if="$1" of=out conv=swab "$3" </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd conv=swab of $1 exited with status $rc"
	cmp -s "$2" out || pc_fail "dd conv=swab $3 of $1 gave the wrong data"
}
chk f6 e6 ibs=512
chk f5 e5 ibs=512
chk f7 e7 ibs=4
pc_pass
