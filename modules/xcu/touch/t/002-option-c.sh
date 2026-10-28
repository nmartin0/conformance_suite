# touch -c creates nothing and writes no diagnostic for a missing file; an existing file is still touched.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.002
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.002
touch -t 200101010101 old || pc_fail "cannot create old"
touch -t 200101010101 ref || pc_fail "cannot create ref"
pc_test
touch -c missing old >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -c exited with status $rc"
[ ! -e missing ] || pc_fail "touch -c created a file"
[ ! -s err ] || pc_fail "diagnostic written for the missing file"
[ old -nt ref ] || pc_fail "the existing file was not touched"
pc_pass
