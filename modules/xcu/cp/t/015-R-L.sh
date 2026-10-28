# cp -R -L copies what a symbolic link in the hierarchy refers to, not the link.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.015
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.015
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

mkdir src || pc_fail "cannot create src"
printf 'x\n' >src/f || pc_fail "cannot create src/f"
ln -s f src/l || pc_fail "cannot create symbolic link"
pc_test
cp -R -L src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -R -L exited with status $rc"
[ ! -h dst/l ] || pc_fail "dst/l is a symbolic link"
[ -f dst/l ] || pc_fail "dst/l is not a regular file"
slurp dst/l
[ "$data" = "x${nl}" ] || pc_fail "dst/l has wrong contents"
pc_pass
