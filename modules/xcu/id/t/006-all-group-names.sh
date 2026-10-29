# id -Gn lists as many names as id -G has IDs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.id.006
. "$PC_CORE/pc.sh"
pc_begin XCU.id.006
LC_ALL=C; export LC_ALL
# Names for IDs that cannot be mapped are not specified for -n; judge only
# when every ID of the default output has a name.
allnamed=yes
for w in $(id | tr ',' ' '); do
	case $w in
	uid=* | euid=* | gid=* | egid=* | groups=* | [0-9]*) case $w in *\(*) ;; *) allnamed=no ;; esac ;;
	esac
done
[ "$allnamed" = yes ] || pc_unresolved "a group ID of this process has no name"
pc_test
ids=$(id -G) || pc_fail "id -G failed"
names=$(id -Gn) || pc_fail "id -Gn failed"
ci=0
for w in $ids; do ci=$((ci + 1)); done
cn=0
for w in $names; do cn=$((cn + 1)); done
[ "$ci" -ge 1 ] || pc_fail "id -G wrote no group ID"
[ "$ci" -eq "$cn" ] || pc_fail "id -G has $ci IDs, id -Gn has $cn words"
en=$(id -gn) || pc_fail "id -gn failed"
[ -n "$en" ] || pc_fail "id -gn wrote no name although every group ID of the process has one"
found=no
for w in $names; do
	[ "$w" = "$en" ] && found=yes
done
[ "$found" = yes ] || pc_fail "the effective group name '$en' is not in '$names'"
pc_pass
