# Without -P, df writes the file system name and exits 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.df.006
. "$PC_CORE/pc.sh"
pc_begin XCU.df.006
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

pc_test
df -P . >out 2>err </dev/null
set -- $(sed -n 2p out)
fsname=$1
[ -n "$fsname" ] || pc_unresolved "setup: no file system name"
df . >out2 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "df exited with status $rc"
slurp out2
case $data in
*"$fsname"*) ;;
*) pc_fail "the output does not contain the file system name '$fsname'" ;;
esac
pc_pass
