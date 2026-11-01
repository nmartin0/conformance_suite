# ln -f replaces an existing destination.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.005
# MUTANT    ln.f_ignored
# MUTANT    ln.f_removes_only
# MUTANT    ln.f_dir_ignored
# MUTANT    ln.f_absent_fails
# MUTANT    ln.f_follows
# MUTANT    ln.ok_status1
# MUTANT    ln.ok_stdout
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
mkdir d || pc_fail "cannot create d"
printf '9\n' >d/a || pc_fail "cannot create d/a"
printf 'k\n' >keep || pc_fail "cannot create keep"
ln -s keep sl || pc_fail "cannot create sl"
pc_test
ln -f a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -f exited with status $rc"
[ ! -s out ] || pc_fail "ln -f wrote to standard output"
[ a -ef b ] || pc_fail "b was not replaced by a link to a"
slurp b
[ "$data" = "1${nl}" ] || pc_fail "b has wrong contents"
ln -f a c >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -f with no existing destination exited with status $rc"
[ a -ef c ] || pc_fail "c is not a link to a"
ln -f a d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -f into a directory exited with status $rc"
[ d/a -ef a ] || pc_fail "d/a was not replaced by a link to a"
ln -f a sl >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -f over a symbolic link exited with status $rc"
[ ! -h sl ] || pc_fail "sl is still a symbolic link"
[ sl -ef a ] || pc_fail "sl is not a link to a"
slurp keep
[ "$data" = "k${nl}" ] || pc_fail "the file the old symbolic link pointed to was changed"
pc_pass
