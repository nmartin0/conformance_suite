# If the destination exists and -f is absent, ln writes a diagnostic, exits non-zero, and leaves the destination alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.004
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.004
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
ln a b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 although b exists"
[ -s err ] || pc_fail "no diagnostic"
[ ! a -ef b ] || pc_fail "b was replaced"
slurp b
[ "$data" = "2${nl}" ] || pc_fail "b was changed"
pc_pass
