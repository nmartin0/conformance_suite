# cp -R source target, with target an existing directory, creates target/source with the hierarchy.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cp.012
. "$PC_CORE/pc.sh"
pc_begin XCU.cp.012
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

mkdir src src/sub src/empty || pc_fail "cannot create tree"
printf 'f1\n' >src/f1 || pc_fail "cannot create src/f1"
printf 'f2\n' >src/sub/f2 || pc_fail "cannot create src/sub/f2"
printf 'h\n' >src/.hid || pc_fail "cannot create src/.hid"
mkdir dst || pc_fail "cannot create dst"
pc_test
cp -R src dst >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "cp -R exited with status $rc"
T=dst/src
[ -d $T/sub ] || pc_fail "$T/sub is not a directory"
[ -d $T/empty ] || pc_fail "$T/empty is not a directory"
slurp $T/f1
[ "$data" = "f1${nl}" ] || pc_fail "$T/f1 missing or wrong"
slurp $T/sub/f2
[ "$data" = "f2${nl}" ] || pc_fail "$T/sub/f2 missing or wrong"
slurp $T/.hid
[ "$data" = "h${nl}" ] || pc_fail "$T/.hid missing or wrong"
pc_pass
