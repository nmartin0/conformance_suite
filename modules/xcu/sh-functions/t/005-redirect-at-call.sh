# Redirections written after a function definition apply at each call, not
# at the definition.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-functions.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-functions.005
pc_test
pcf() { echo hi; } >pc.fo
[ -e pc.fo ] && pc_fail "the file was made at definition"
pcf
pcf
[ "$(cat pc.fo)" = hi ] || pc_fail "after two calls the file holds '$(cat pc.fo)', expected one hi"
pc_pass
