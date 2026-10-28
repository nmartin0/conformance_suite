# ls -c: status change time for sorting and writing; -c, -u and -S with -t
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.040
# Three files with three different orders: by modification time a1 c1 b1, by
# status change time c1 b1 a1 (created in that order, 2 s apart), by size
# b1 a1 c1.
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.040
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
printf 'aaaaa' >d/a1 && touch -t 200501010000 d/a1 || pc_unresolved "cannot create a1"
sleep 2
printf 'bbbbbbbbbb' >d/b1 && touch -t 200101010000 d/b1 || pc_unresolved "cannot create b1"
sleep 2
printf 'c' >d/c1 && touch -t 200301010000 d/c1 || pc_unresolved "cannot create c1"
pc_test
# order <expected> <options...>
order() {
	w=$1; shift
	got=$(ls "$@" d 2>err </dev/null | tr '\n' ' ')
	[ "$got" = "$w " ] || pc_fail "ls $*: '$got', expected '$w'"
}
order 'a1 c1 b1' -t
order 'c1 b1 a1' -tc
order 'b1 a1 c1' -t -c -S
order 'b1 a1 c1' -S -c
for o in "-l -S -c" "-l -t -S -c" "-l -t -c -S" "-lc"; do
	ls $o d >out 2>err </dev/null || pc_fail "ls $o failed"
	grep ' 20[0-9][0-9] ' out >/dev/null && pc_fail "ls $o wrote a year of the modification time instead of the status change time: $(cat out)"
done
ls -l d >out 2>err </dev/null || pc_fail "ls -l failed"
n=$(grep -c ' 200[135] ' out)
[ "$n" -eq 3 ] || pc_fail "ls -l did not write the modification times (years found for $n of 3 files)"
pc_pass
