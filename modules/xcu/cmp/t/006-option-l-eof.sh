# With -l, differing lengths give the EOF diagnostic after any differences have been listed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.006
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.006
nl='
'
slurp() {
	data=
	l=
	while IFS= read -r l; do
		data=$data$l$nl
		l=
	done <"$1"
	[ -z "$l" ] || data=$data$l'<noeol>'
	return 0
}

printf 'abc' >f1 || pc_fail "cannot create f1"
printf 'abcd' >f2 || pc_fail "cannot create f2"
printf 'aXc' >g1 || pc_fail "cannot create g1"
pc_test
cmp -l f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l exited with status $rc, expected 1"
e=$(cat err)
case $e in
"cmp: EOF on f1"|"cmp: EOF on f1 "*) ;;
*) pc_fail "diagnostic is: $e" ;;
esac
cmp -l g1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp -l (second case) exited with status $rc, expected 1"
slurp out
[ "$data" = "2 130 142${nl}" ] || pc_fail "output is: $data"
e=$(cat err)
case $e in
"cmp: EOF on g1"|"cmp: EOF on g1 "*) ;;
*) pc_fail "second diagnostic is: $e" ;;
esac
pc_pass
