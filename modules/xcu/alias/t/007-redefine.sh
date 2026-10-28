# A second definition replaces the first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.alias.007
. "$PC_CORE/pc.sh"
pc_begin XCU.alias.007
pc_test
alias pcal7='echo one'
alias pcal7='echo two'
got=$(eval pcal7)
[ "$got" = two ] || pc_fail "substituted '$got', wanted two"
shown=$(alias pcal7)
case $shown in
*two*) ;;
*) pc_fail "alias pcal7 does not show the new value: $shown" ;;
esac
case $shown in
*one*) pc_fail "alias pcal7 still shows the old value: $shown" ;;
esac
pc_pass
