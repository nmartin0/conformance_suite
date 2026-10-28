# ln -f replaces an existing destination.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.005
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.005
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

printf '1\n' >a || pc_fail "cannot create a"
printf '2\n' >b || pc_fail "cannot create b"
pc_test
ln -f a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -f exited with status $rc"
[ a -ef b ] || pc_fail "b was not replaced by a link to a"
slurp b
[ "$data" = "1${nl}" ] || pc_fail "b has wrong contents"
pc_pass
