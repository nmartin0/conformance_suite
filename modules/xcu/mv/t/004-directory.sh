# mv renames a directory with everything below it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.004
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.004
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

mkdir -p d/sub || pc_fail "cannot create d/sub"
printf '1\n' >d/f || pc_fail "cannot create d/f"
printf '2\n' >d/sub/g || pc_fail "cannot create d/sub/g"
pc_test
mv d e >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv exited with status $rc"
[ ! -e d ] || pc_fail "source directory still exists"
slurp e/f
[ "$data" = "1${nl}" ] || pc_fail "e/f missing or wrong"
slurp e/sub/g
[ "$data" = "2${nl}" ] || pc_fail "e/sub/g missing or wrong"
pc_pass
