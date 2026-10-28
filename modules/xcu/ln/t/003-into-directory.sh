# ln source_file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.003
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.003
linkof() {
	lk=$(ls -ld "$1") || return 1
	lk=${lk##* -> }
}

printf '1\n' >a || pc_fail "cannot create a"
printf '2\n' >b || pc_fail "cannot create b"
mkdir d e || pc_fail "cannot create directories"
pc_test
ln a b d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln exited with status $rc"
[ d/a -ef a ] || pc_fail "d/a is not a link to a"
[ d/b -ef b ] || pc_fail "d/b is not a link to b"
ln -s a b e >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s exited with status $rc"
linkof e/a || pc_fail "cannot list e/a"
[ "$lk" = a ] || pc_fail "e/a contains '$lk'"
linkof e/b || pc_fail "cannot list e/b"
[ "$lk" = b ] || pc_fail "e/b contains '$lk'"
pc_pass
