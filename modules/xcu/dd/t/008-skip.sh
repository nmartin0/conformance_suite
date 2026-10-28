# dd skip= on a regular file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.008
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# skip=n skips n input blocks; it combines with count=. Both for if= and
# for a regular file on standard input.
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
gen 0 10 >f1000 || pc_fail "cannot write f1000"
gen 3 7 >exp37 || pc_fail "cannot write exp37"
gen 3 10 >exp310 || pc_fail "cannot write exp310"
pc_test
dd if=f1000 of=out ibs=100 skip=3 count=4 </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd skip=3 count=4 exited with status $rc"
cmp -s exp37 out || pc_fail "skip=3 count=4 ibs=100 did not copy bytes 300 to 699"
dd ibs=100 skip=3 <f1000 >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd skip=3 on standard input exited with status $rc"
cmp -s exp310 out || pc_fail "skip=3 ibs=100 on standard input did not copy from byte 300"
pc_pass
