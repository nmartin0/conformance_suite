# return ends only the function it is in; a calling function carries on.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.return.006
. "$PC_CORE/pc.sh"
pc_begin XCU.return.006
pc_test
pcg() { return 2; }
pcf() { pcg; echo "g=$?"; return 6; }
r=$(pcf)
[ "$r" = g=2 ] || pc_fail "got '$r', expected g=2"
pcf >/dev/null
rc=$?
[ "$rc" -eq 6 ] || pc_fail "outer status $rc, expected 6"
pc_pass
