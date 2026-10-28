# ln -s creates a symbolic link whose contents are the source operand text exactly, whether or not the source exists.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.002
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.002
linkof() {
	lk=$(ls -ld "$1") || return 1
	lk=${lk##* -> }
}

pc_test
ln -s missing.txt s1 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s exited with status $rc"
[ -h s1 ] || pc_fail "s1 is not a symbolic link"
linkof s1 || pc_fail "cannot list s1"
[ "$lk" = missing.txt ] || pc_fail "contents are '$lk', expected missing.txt"
ln -s ../x/./y s2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s with a relative text exited with status $rc"
linkof s2 || pc_fail "cannot list s2"
[ "$lk" = ../x/./y ] || pc_fail "contents are '$lk', expected ../x/./y"
pc_pass
