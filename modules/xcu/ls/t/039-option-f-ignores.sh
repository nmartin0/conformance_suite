# ls -f turns on -a and leaves -r, -t and -S without effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.039
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.039
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
printf 'aaaaaaaaaa' >d/a && printf 'b' >d/b && printf 'cccccc' >d/c && : >d/.h || pc_unresolved "cannot create the files"
touch -t 200301010000 d/a && touch -t 200101010000 d/b && touch -t 200201010000 d/c || pc_unresolved "touch failed"
pc_test
ls -f d >o0 2>err </dev/null || pc_fail "ls -f failed"
got=$(sort o0 | tr '\n' ' ')
[ "$got" = '. .. .h a b c ' ] || pc_fail "ls -f lists '$got', expected dot, dot-dot, .h, a, b and c"
for o in -r -t -S -rtS -Sr; do
	ls -f $o d >o1 2>err </dev/null || pc_fail "ls -f $o failed"
	cmp -s o0 o1 || pc_fail "ls -f $o gives another order than ls -f: '$(tr '\n' ' ' <o1)' against '$(tr '\n' ' ' <o0)'"
	ls $o -f d >o2 2>err </dev/null || pc_fail "ls $o -f failed"
	cmp -s o0 o2 || pc_fail "ls $o -f gives another order than ls -f: '$(tr '\n' ' ' <o2)' against '$(tr '\n' ' ' <o0)'"
done
pc_pass
