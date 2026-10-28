# id NAME for the real user, run with privilege.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.007
. "$PC_CORE/pc.sh"
pc_begin XCU.id.007
LC_ALL=C; export LC_ALL
name=$(id -urn) || pc_fail "id -urn failed"
[ -n "$name" ] || pc_unresolved "the real user ID has no name"
ru=$(id -ur)
pc_test
x=$(id -u "$name") || pc_fail "id -u $name failed"
[ "$x" = "$ru" ] || pc_fail "id -u $name is '$x', the real user ID is '$ru'"
out=$(id "$name") || pc_fail "id $name failed"
set -- $out
case $1 in
"uid=$ru"*) ;;
*) pc_fail "id $name starts with '$1', expected uid=$ru" ;;
esac
for w in "$@"; do
	case $w in
	euid=* | egid=*) pc_fail "id $name wrote '$w', effective IDs are the real ones" ;;
	esac
done
pc_pass
