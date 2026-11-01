# ln source_file... target_dir creates target_dir/basename for each source, as hard links or, with -s, as links holding the operand text.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.003
# MUTANT    ln.dir_first_only
# MUTANT    ln.dir_keeps_path
# MUTANT    ln.dir_sym_basename
# MUTANT    ln.dir_sym_hard
# MUTANT    ln.dir_trailing_slash_fail
# MUTANT    ln.dir_symlink_not_dir
# MUTANT    ln.ok_status1
# MUTANT    ln.ok_stdout
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.003
linkof() {
	lk=$(ls -ld "$1") || return 1
	lk=${lk##* -> }
}

printf '1\n' >a || pc_fail "cannot create a"
printf '2\n' >b || pc_fail "cannot create b"
mkdir d e g g2 d2 sub || pc_fail "cannot create directories"
printf '3\n' >sub/x || pc_fail "cannot create sub/x"
ln -s d sld || pc_fail "cannot create sld"
pc_test
ln a b d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln exited with status $rc"
[ ! -s out ] || pc_fail "ln wrote to standard output"
[ d/a -ef a ] || pc_fail "d/a is not a link to a"
[ d/b -ef b ] || pc_fail "d/b is not a link to b"
[ ! -h d/a ] || pc_fail "d/a is a symbolic link"
ln -s a b e >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s exited with status $rc"
[ ! -s out ] || pc_fail "ln -s wrote to standard output"
[ -h e/a ] || pc_fail "e/a is not a symbolic link"
linkof e/a || pc_fail "cannot list e/a"
[ "$lk" = a ] || pc_fail "e/a contains '$lk'"
[ -h e/b ] || pc_fail "e/b is not a symbolic link"
linkof e/b || pc_fail "cannot list e/b"
[ "$lk" = b ] || pc_fail "e/b contains '$lk'"
ln sub/x g >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln of sub/x into g exited with status $rc"
[ g/x -ef sub/x ] || pc_fail "g/x is not a link to sub/x"
[ ! -e g/sub ] || pc_fail "the directory part of the source was kept in the name"
ln -s sub/x g2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s of sub/x into g2 exited with status $rc"
[ -h g2/x ] || pc_fail "g2/x is not a symbolic link"
linkof g2/x || pc_fail "cannot list g2/x"
[ "$lk" = sub/x ] || pc_fail "g2/x contains '$lk', expected sub/x"
ln a d2/ >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln with a target directory ending in a slash exited with status $rc"
[ d2/a -ef a ] || pc_fail "d2/a is not a link to a"
ln sub/x sld >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln into a symbolic link to a directory exited with status $rc"
[ d/x -ef sub/x ] || pc_fail "d/x was not made through the symbolic link to d"
pc_pass
