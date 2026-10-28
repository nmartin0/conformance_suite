# unset accepts "--" as the end of options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unset.007
. "$PC_CORE/pc.sh"
pc_begin XCU.unset.007
pc_test
PCU8=1
unset -- PCU8
[ -z "${PCU8+set}" ] || pc_fail "PCU8 still set"
pc_pass
