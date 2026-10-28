# split creates no file for an empty input and that is not an error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.split.007
. "$PC_CORE/pc.sh"
pc_begin XCU.split.007
LC_ALL=POSIX
export LC_ALL
NL='
'
: >empty || pc_fail "cannot create input"
pc_test
split empty >/dev/null 2>&1 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "split on an empty file gave status $rc"
[ -e xaa ] && pc_fail "a file was created for empty input"
pc_pass
