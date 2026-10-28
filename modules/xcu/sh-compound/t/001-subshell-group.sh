# ( list ) runs the list in a subshell: its assignments and directory
# changes do not remain, and its status is that of the list.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-compound.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-compound.001
mkdir pc.sub || pc_unresolved "mkdir"
PCV=out
PCK=keep
d0=$(pwd -P)
pc_test
( PCV=in; cd pc.sub; unset PCK )
[ "$PCV" = out ] || pc_fail "assignment remained: PCV='$PCV'"
[ "$PCK" = keep ] || pc_fail "unset remained"
[ "$(pwd -P)" = "$d0" ] || pc_fail "cd remained"
( exit 5 )
[ "$?" -eq 5 ] || pc_fail "status of ( exit 5 ) was $?"
( false; true )
[ "$?" -eq 0 ] || pc_fail "status of ( false; true ) was $?"
r=$( (echo a; echo b) )
[ "$r" = "a
b" ] || pc_fail "output of the group: '$r'"
pc_pass
