# dd conv=unblock
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.017
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# cbs bytes at a time (or what is left), trailing spaces deleted, a newline
# appended; independent of the input block size.
printf 'ab  cd  efghxy' >f1 || pc_fail "cannot write f1"
printf 'ab\ncd\nefgh\nxy\n' >e1 || pc_fail "cannot write e1"
printf '    ab  ' >f2 || pc_fail "cannot write f2"
printf '\nab\n' >e2 || pc_fail "cannot write e2"
printf 'a b ' >f3 || pc_fail "cannot write f3"
printf 'a b\n' >e3 || pc_fail "cannot write e3"
pc_test
run() {
	dd if="$1" of=out cbs=4 conv=unblock "$3" </dev/null 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "dd conv=unblock $3 of $1 exited with status $rc"
	cmp -s "$2" out || pc_fail "dd conv=unblock cbs=4 $3 of $1: wrong output data"
}
for ib in ibs=512 ibs=3; do
	run f1 e1 $ib
	run f2 e2 $ib
	run f3 e3 $ib
done
pc_pass
