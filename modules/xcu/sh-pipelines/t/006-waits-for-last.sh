# The shell waits for the last command of a pipeline to finish before going
# on.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-pipelines.006
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-pipelines.006
pc_test
true | { sleep 1; : >pc.w; }
[ -f pc.w ] || pc_fail "the shell went on before the last command finished"
pc_pass
