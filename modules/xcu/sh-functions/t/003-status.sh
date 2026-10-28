# A function call has the status of the last command run in it; the status
# of a definition is zero.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.003
pc_test
pcf() { false; }
pcf
[ "$?" -eq 1 ] || pc_fail "false: $?"
pcf() { (exit 5); }
pcf
[ "$?" -eq 5 ] || pc_fail "(exit 5): $?"
pcf() { true; }
pcf
[ "$?" -eq 0 ] || pc_fail "true: $?"
pcf() { return 3; }
pcf
[ "$?" -eq 3 ] || pc_fail "return 3: $?"
false
pcf() { :; }
[ "$?" -eq 0 ] || pc_fail "status of the definition: $?"
pc_pass
