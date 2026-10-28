# ls -a also lists names that start with a period.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.002
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.002
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
for n in b a C .h; do
	: >"d/$n" || pc_fail "cannot create d/$n"
done
pc_test
ls -a d >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -a exited with status $rc"
got=
while IFS= read -r l; do
	case $l in
	.|..) ;;
	*) got=$got$l$nl ;;
	esac
done <out
[ "$got" = ".h${nl}C${nl}a${nl}b${nl}" ] || pc_fail "ls -a lists the wrong entries or in the wrong order"
pc_pass
