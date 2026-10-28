# The last of -f and -i decides: -i -f does not prompt, -f -i does.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.007
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.007
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

printf 'new1\n' >a || pc_fail "cannot create a"
printf 'old1\n' >b || pc_fail "cannot create b"
printf 'new2\n' >c || pc_fail "cannot create c"
printf 'old2\n' >d || pc_fail "cannot create d"
pc_test
printf 'n\n' | mv -i -f a b >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv -i -f exited with status $rc"
[ ! -e a ] || pc_fail "-i -f: the source was not moved"
slurp b
[ "$data" = "new1${nl}" ] || pc_fail "-i -f: destination not replaced"
printf 'n\n' | mv -f -i c d >out 2>err
rc=$?
[ -e c ] || pc_fail "-f -i: the source was moved although the reply was negative"
slurp d
[ "$data" = "old2${nl}" ] || pc_fail "-f -i: destination was replaced"
pc_pass
