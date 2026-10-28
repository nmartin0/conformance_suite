# An assignment before a function call is in effect while the function runs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-simple-commands.004
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-simple-commands.004
pc_test
pcf() { printf %s "$PCV"; }
r=$(PCV=fn pcf)
[ "$r" = fn ] || pc_fail "function saw '$r', expected fn"
pc_pass
