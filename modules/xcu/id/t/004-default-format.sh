# Shape of the default output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.004
. "$PC_CORE/pc.sh"
pc_begin XCU.id.004
LC_ALL=C; export LC_ALL
# item WORD PREFIX: the rest of WORD after PREFIX must be ID or ID(name).
item() {
	r=${1#"$2"}
	case $r in
	*\(*\)) n=${r%%\(*}; nm=${r#*\(}; nm=${nm%\)}; [ -n "$nm" ] || pc_fail "'$1' has an empty name" ;;
	*\(* | *\)*) pc_fail "'$1' has unbalanced parentheses" ;;
	*) n=$r ;;
	esac
	case $n in
	'' | *[!0-9]*) pc_fail "'$1' does not hold a decimal ID" ;;
	esac
}
pc_test
out=$(id) || pc_fail "id failed"
case $out in
*"  "* | " "* | *" ") pc_fail "'$out' has stray blanks" ;;
esac
set -- $out
item "$1" uid=
item "$2" gid=
shift 2
ru=$(id -ur); rg=$(id -gr); eu=$(id -u); eg=$(id -g)
wantu=no wantg=no
[ "$ru" = "$eu" ] || wantu=yes
[ "$rg" = "$eg" ] || wantg=yes
haveu=no haveg=no
if [ "$#" -gt 0 ]; then
	case $1 in
	euid=*) item "$1" euid=; haveu=yes; [ "$n" = "$eu" ] || pc_fail "euid= is $n, id -u is $eu"; shift ;;
	esac
fi
if [ "$#" -gt 0 ]; then
	case $1 in
	egid=*) item "$1" egid=; haveg=yes; [ "$n" = "$eg" ] || pc_fail "egid= is $n, id -g is $eg"; shift ;;
	esac
fi
[ "$haveu" = "$wantu" ] || pc_fail "euid= present: $haveu, effective and real user IDs differ: $wantu"
[ "$haveg" = "$wantg" ] || pc_fail "egid= present: $haveg, effective and real group IDs differ: $wantg"
if [ "$#" -gt 0 ]; then
	[ "$#" -eq 1 ] || pc_fail "unexpected words after the group items: $*"
	case $1 in
	groups=*) ;;
	*) pc_fail "unexpected word '$1'" ;;
	esac
	list=${1#groups=}
	[ -n "$list" ] || pc_fail "groups= is empty"
	all=$(id -G) || pc_fail "id -G failed"
	oldifs=$IFS
	IFS=,
	set -f
	set -- $list
	set +f
	IFS=$oldifs
	for it in "$@"; do
		item "$it" ""
		found=no
		for x in $all; do
			[ "$x" = "$n" ] && found=yes
		done
		[ "$found" = yes ] || pc_fail "group $n under groups= is not in id -G ($all)"
	done
fi
pc_pass
