# The -n names agree with the names in the default output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.003
. "$PC_CORE/pc.sh"
pc_begin XCU.id.003
LC_ALL=C; export LC_ALL
# nm WORD: v = the name in "ID(name)", empty when there is none.
nm() {
	case $1 in
	*\(*) v=${1#*\(}; v=${v%\)} ;;
	*) v= ;;
	esac
}
pc_test
out=$(id) || pc_fail "id failed"
set -- $out
case $1 in uid=*) ;; *) pc_fail "first word '$1' is not uid=" ;; esac
case $2 in gid=*) ;; *) pc_fail "second word '$2' is not gid=" ;; esac
nm "$1"; run=$v
nm "$2"; rgn=$v
eun=$run egn=$rgn
shift 2
for w in "$@"; do
	case $w in
	euid=*) nm "$w"; eun=$v ;;
	egid=*) nm "$w"; egn=$v ;;
	esac
done
checked=0
if [ -n "$run" ]; then
	checked=1
	x=$(id -urn) || pc_fail "id -urn failed"
	[ "$x" = "$run" ] || pc_fail "id -urn is '$x', the default output shows '$run'"
	x=$(id -ur -n) || pc_fail "id -ur -n failed"
	[ "$x" = "$run" ] || pc_fail "id -ur -n is '$x', the default output shows '$run'"
fi
if [ -n "$rgn" ]; then
	checked=1
	x=$(id -grn) || pc_fail "id -grn failed"
	[ "$x" = "$rgn" ] || pc_fail "id -grn is '$x', the default output shows '$rgn'"
fi
if [ -n "$eun" ]; then
	checked=1
	x=$(id -un) || pc_fail "id -un failed"
	[ "$x" = "$eun" ] || pc_fail "id -un is '$x', the default output shows '$eun'"
fi
if [ -n "$egn" ]; then
	checked=1
	x=$(id -gn) || pc_fail "id -gn failed"
	[ "$x" = "$egn" ] || pc_fail "id -gn is '$x', the default output shows '$egn'"
fi
[ "$checked" -eq 1 ] || pc_unresolved "no ID of this process maps to a name"
pc_pass
