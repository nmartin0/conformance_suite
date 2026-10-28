# cmp compares bytes of any value, null bytes included, and accepts a character special file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cmp.009
. "$PC_CORE/pc.sh"
pc_begin XCU.cmp.009
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

printf 'a\000b' >f1 || pc_fail "cannot create f1"
printf 'a\000c' >f2 || pc_fail "cannot create f2"
printf 'a\000b' >f3 || pc_fail "cannot create f3"
: >e || pc_fail "cannot create e"
pc_test
cmp f1 f3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp exited with status $rc for equal files with a null byte"
cmp f1 f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "cmp exited with status $rc for files differing after a null byte"
slurp out
[ "$data" = "f1 f2 differ: char 3, line 1${nl}" ] || pc_fail "output is: $data"
cmp e /dev/null >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cmp e /dev/null exited with status $rc"
pc_pass
