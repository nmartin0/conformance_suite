# command -p finds standard utilities by the default path.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.010
. "$PC_CORE/pc.sh"
pc_begin XCU.command.010
printf 'data\n' >in || pc_fail "cannot write a file"
pc_test
got=$( PATH=$PWD/nodir; command -p cat in 2>/dev/null )
[ "$got" = data ] || pc_fail "command -p cat gave '$got' with PATH naming no directory, wanted data"
pc_pass
