# With -i, an existing destination triggers a prompt naming it on standard error, and a negative reply leaves it untouched with status 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.009
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.009
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

printf 'new\n' >src || pc_fail "cannot create src"
printf 'old\n' >dstfile || pc_fail "cannot create dstfile"
pc_test
printf 'n\n' | cp -i src dstfile >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -i exited with status $rc after a negative reply"
slurp dstfile
[ "$data" = "old${nl}" ] || pc_fail "destination was overwritten"
slurp err
case $data in
*dstfile*) ;;
*) pc_fail "the prompt on standard error does not contain the destination name" ;;
esac
pc_pass
