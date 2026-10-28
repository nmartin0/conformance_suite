# asa exits above zero for a file it cannot read.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.asa.006
. "$PC_CORE/pc.sh"
pc_begin XCU.asa.006
LC_ALL=POSIX
export LC_ALL
NL='
'
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
asa nosuchfile >/dev/null 2>&1 </dev/null
[ $? -gt 0 ] || pc_fail "status 0 for a missing file"
pc_pass
