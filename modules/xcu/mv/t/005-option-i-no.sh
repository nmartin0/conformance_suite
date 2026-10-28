# With -i, an existing destination triggers a prompt naming it on standard error; a negative reply leaves both files alone.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.mv.005
. "$PC_CORE/pc.sh"
pc_begin XCU.mv.005
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

printf 'new\n' >a || pc_fail "cannot create a"
printf 'old\n' >bfile || pc_fail "cannot create bfile"
pc_test
printf 'n\n' | mv -i a bfile >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "mv -i exited with status $rc after a negative reply"
slurp a
[ "$data" = "new${nl}" ] || pc_fail "source was moved"
slurp bfile
[ "$data" = "old${nl}" ] || pc_fail "destination was replaced"
slurp err
case $data in
*bfile*) ;;
*) pc_fail "the prompt does not contain the destination name" ;;
esac
pc_pass
