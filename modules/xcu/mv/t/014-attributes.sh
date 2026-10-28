# mv keeps the modification time and permission bits of the file it moves.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.014
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.014
modeof() {
	mo=$(ls -ld "$1" 2>/dev/null) || return 1
	mo=${mo%% *}
	mo=${mo%"${mo#??????????}"}
}

printf 'x\n' >a || pc_fail "cannot create a"
chmod 640 a || pc_fail "cannot chmod a"
touch -t 200101020304 a ref || pc_fail "cannot set times"
pc_test
mv a b >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv exited with status $rc"
modeof b || pc_fail "cannot list b"
[ "$mo" = "-rw-r-----" ] || pc_fail "mode is $mo, expected -rw-r-----"
[ ! b -nt ref ] || pc_fail "b is newer than the original time"
[ ! ref -nt b ] || pc_fail "b is older than the original time"
pc_pass
