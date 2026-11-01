# If the destination exists and -f is absent, ln writes a diagnostic, exits non-zero, and leaves the destination alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.004
# MUTANT    ln.exists_overwrite
# MUTANT    ln.exists_clobbers
# MUTANT    ln.err_silent
# MUTANT    ln.err_status0
# VARIANT   ln.status2
# VARIANT   ln.status255
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
mkdir d || pc_fail "cannot create d"
printf '9\n' >d/a || pc_fail "cannot create d/a"
pc_test
ln a b >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 although b exists"
[ -s err ] || pc_fail "no diagnostic"
[ ! a -ef b ] || pc_fail "b was replaced"
[ -f b ] || pc_fail "b is gone"
slurp b
[ "$data" = "2${nl}" ] || pc_fail "b was changed"
ln a d >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln exited 0 although d/a exists"
[ -s err ] || pc_fail "no diagnostic for the directory form"
[ ! d/a -ef a ] || pc_fail "d/a was replaced"
slurp d/a
[ "$data" = "9${nl}" ] || pc_fail "d/a was changed"
pc_pass
