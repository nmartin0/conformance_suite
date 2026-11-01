# ln -s creates a symbolic link whose contents are the source operand text exactly, whether or not the source exists.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.002
# MUTANT    ln.sym_abs
# MUTANT    ln.sym_normalized
# MUTANT    ln.sym_needs_exist
# MUTANT    ln.sym_hard_existing
# MUTANT    ln.ok_status1
# MUTANT    ln.ok_stdout
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.002
linkof() {
	lk=$(ls -ld "$1") || return 1
	lk=${lk##* -> }
}

printf 'real\n' >real || pc_fail "cannot create real"
pc_test
ln -s missing.txt s1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s exited with status $rc"
[ ! -s out ] || pc_fail "ln -s wrote to standard output"
[ -h s1 ] || pc_fail "s1 is not a symbolic link"
linkof s1 || pc_fail "cannot list s1"
[ "$lk" = missing.txt ] || pc_fail "contents are '$lk', expected missing.txt"
ln -s ../x/./y s2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s with a relative text exited with status $rc"
linkof s2 || pc_fail "cannot list s2"
[ "$lk" = ../x/./y ] || pc_fail "contents are '$lk', expected ../x/./y"
ln -s real s3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s of an existing source exited with status $rc"
[ -h s3 ] || pc_fail "s3 is not a symbolic link"
linkof s3 || pc_fail "cannot list s3"
[ "$lk" = real ] || pc_fail "contents are '$lk', expected real"
pc_pass
