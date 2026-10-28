# An input file that cannot be read gives a nonzero status and no output files.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.009
. "$PC_CORE/pc.sh"
pc_begin XCU.split.009
LC_ALL=POSIX
export LC_ALL
NL='
'
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
split nosuchfile >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for an input file that cannot be read"
[ -e xaa ] && pc_fail "a file was created from an unreadable input"
pc_pass
