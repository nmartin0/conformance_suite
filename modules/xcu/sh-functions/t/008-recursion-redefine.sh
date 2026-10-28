# A function can call itself, and a later definition replaces an earlier
# one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.008
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.008
pc_test
pcf() { if [ "$1" -le 0 ]; then return 0; fi; printf %s "$1"; pcf $(($1 - 1)); }
r=$(pcf 3)
[ "$r" = 321 ] || pc_fail "recursion: '$r'"
pcg() { echo one; }
pcg() { echo two; }
r=$(pcg)
[ "$r" = two ] || pc_fail "redefinition: '$r'"
pc_pass
