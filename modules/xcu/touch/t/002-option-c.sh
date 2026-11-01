# touch -c creates nothing and writes no diagnostic for a missing file; an existing file is still touched.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.touch.002
# MUTANT    touch.c_creates
# MUTANT    touch.c_diag
# MUTANT    touch.c_status1
# MUTANT    touch.c_stops_first_missing
# MUTANT    touch.c_nothing
# MUTANT    touch.c_inverted
. "$PC_CORE/pc.sh"
pc_begin XCU.touch.002
TZ=UTC0; export TZ
same() { [ -e "$1" ] && [ -e "$2" ] && [ ! "$1" -nt "$2" ] && [ ! "$2" -nt "$1" ]; }
touch -t 200101010101 old || pc_fail "cannot create old"
touch -t 200101010101 ref || pc_fail "cannot create ref"
touch -t 200101010101 old2 || pc_fail "cannot create old2"
touch -t 200202020202 ref2 || pc_fail "cannot create ref2"
pc_test
touch -c missing old >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -c exited with status $rc"
[ ! -e missing ] || pc_fail "touch -c created a file"
[ ! -s err ] || pc_fail "diagnostic written for the missing file"
[ -e old ] || pc_fail "old vanished"
[ old -nt ref ] || pc_fail "the existing file was not touched"
touch -c -t 200202020202 missing2 old2 >out2 2>err2 </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "touch -c -t exited with status $rc"
[ ! -e missing2 ] || pc_fail "touch -c -t created a file"
[ ! -s err2 ] || pc_fail "diagnostic written for the missing file with -t"
same old2 ref2 || pc_fail "the existing file was not given the -t time"
pc_pass
