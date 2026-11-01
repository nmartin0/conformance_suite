# ln -L makes a hard link to the file a symbolic link refers to.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.008
# MUTANT    ln.L_as_P
# MUTANT    ln.L_regular_fails
# MUTANT    ln.L_resolves_cwd
# MUTANT    ln.L_dir_wrong_name
# MUTANT    ln.ok_status1
# MUTANT    ln.ok_stdout
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.008
printf 'd\n' >lf8 || pc_fail "cannot create lf8"
ln -s lf8 sl || pc_fail "cannot create symbolic link"
mkdir s d || pc_fail "cannot create directories"
ln -s ../lf8 s/sl || pc_fail "cannot create symbolic link in s"
pc_test
ln -L sl h >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -L exited with status $rc"
[ ! -s out ] || pc_fail "ln -L wrote to standard output"
[ ! -h h ] || pc_fail "h is a symbolic link"
[ h -ef lf8 ] || pc_fail "h is not a link to lf8"
ln -L lf8 h2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -L of a regular file exited with status $rc"
[ h2 -ef lf8 ] || pc_fail "h2 is not a link to lf8"
ln -L s/sl h3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -L of a relative symbolic link in s exited with status $rc"
[ ! -h h3 ] || pc_fail "h3 is a symbolic link"
[ h3 -ef lf8 ] || pc_fail "h3 is not a link to lf8"
ln -L sl d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -L into a directory exited with status $rc"
[ -e d/sl ] || pc_fail "d/sl was not made"
[ ! -h d/sl ] || pc_fail "d/sl is a symbolic link"
[ d/sl -ef lf8 ] || pc_fail "d/sl is not a link to lf8"
pc_pass
