# With several sources, a target that is not a directory is an error and the target is not modified.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.021
# MUTANT    cp.multi_t_overwrites
# MUTANT    cp.multi_t_modifies
# MUTANT    cp.multi_t_status0
# MUTANT    cp.multi_t_missing_ok
# MUTANT    cp.no_diag
# MUTANT    cp.diag_stdout
# VARIANT    cp.status2
# VARIANT    cp.status255
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.021
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

printf 'A\n' >a || pc_fail "cannot create a"
printf 'B\n' >b || pc_fail "cannot create b"
printf 'orig\n' >c || pc_fail "cannot create c"
pc_test
cp a b c >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp exited 0 for a regular file target with two sources"
[ -s err ] || pc_fail "no diagnostic for a regular file target with two sources"
slurp c
[ "$data" = "orig${nl}" ] || pc_fail "target was modified"
cp a b newtarget >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "cp exited 0 for a missing target with two sources"
[ -s err ] || pc_fail "no diagnostic for a missing target with two sources"
pc_pass
