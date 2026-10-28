# ls -n shows the numeric user ID and a numeric group ID instead of names.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.013
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.013
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_fail "cannot create d"
: >d/f || pc_fail "cannot create d/f"
uid=$(id -u) || pc_fail "cannot determine the user ID"
pc_test
ls -n d/f >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -n exited with status $rc"
set -- $(cat out)
[ "$#" -eq 9 ] || pc_unresolved "unexpected number of fields ($#)"
[ "$3" = "$uid" ] || pc_fail "owner field is '$3', expected $uid"
case $4 in
''|*[!0-9]*) pc_fail "group field '$4' is not numeric" ;;
esac
case $1 in
-*) ;;
*) pc_fail "ls -n did not write the long format" ;;
esac
pc_pass
