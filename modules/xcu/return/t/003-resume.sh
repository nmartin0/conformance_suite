# After return, execution resumes with the command after the function call.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.return.003
. "$PC_CORE/pc.sh"
pc_begin XCU.return.003
pc_test
pcf() { echo in; return; echo notrun; }
r=$(pcf; echo next)
nl='
'
[ "$r" = "in${nl}next" ] || pc_fail "got '$r'"
pc_pass
