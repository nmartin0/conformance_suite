# -C, -x and -m turn long format off and a later -l turns it on again: the total line shows which one wins.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.035
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.035
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

LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
for n in a b; do
	: >"d/$n" || pc_fail "cannot create d/$n"
done
pc_test
for o in -C -x -m; do
	ls -l $o d >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "ls -l $o exited with status $rc"
	if grep '^total' out >/dev/null; then
		pc_fail "ls -l $o still wrote the long format"
	fi
	slurp out
	case $data in
	*a*b*) ;;
	*) pc_fail "ls -l $o did not list both names" ;;
	esac
	ls $o -l d >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "ls $o -l exited with status $rc"
	grep '^total' out >/dev/null || pc_fail "ls $o -l did not write the long format"
done
pc_pass
