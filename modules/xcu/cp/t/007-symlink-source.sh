# Without -R, a source that is a symbolic link is copied as the file it refers to, not as a link.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.007
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.007
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

printf 'data\n' >real || pc_fail "cannot create real"
ln -s real lnk || pc_fail "cannot create symbolic link"
pc_test
cp lnk copy >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp exited with status $rc"
[ ! -h copy ] || pc_fail "the copy is a symbolic link"
[ -f copy ] || pc_fail "the copy is not a regular file"
slurp copy
[ "$data" = "data${nl}" ] || pc_fail "copy has wrong contents"
pc_pass
