# cp -f replaces a destination file that cannot be opened for writing, by removing it first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.008
#
# As a privileged user the destination can be opened anyway, so the test
# only distinguishes a faulty -f when run unprivileged; the verdict for
# a conforming utility is the same in both cases.
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.008
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

printf 'old\n' >dst || pc_fail "cannot create dst"
chmod 400 dst || pc_fail "cannot chmod dst"
printf 'new\n' >src || pc_fail "cannot create src"
pc_test
cp -f src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -f exited with status $rc"
slurp dst
[ "$data" = "new${nl}" ] || pc_fail "destination was not replaced"
pc_pass
