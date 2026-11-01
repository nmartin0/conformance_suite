# nm of a nonexistent file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.010
# "nm" on a file that does not exist writes a diagnostic to standard
# error and exits with a status greater than zero; the other operands are
# still processed (in either order) and still listed.
# MUTANT    nm.miss_status0
# MUTANT    nm.miss_no_diag
# MUTANT    nm.miss_diag_stdout
# MUTANT    nm.miss_abort
# MUTANT    nm.miss_status_last
# MUTANT    nm.miss_status_first
# MUTANT    nm.miss_discard
# VARIANT   nm.status2
# VARIANT   nm.status255
# VARIANT   nm.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_notinuse "c17 is not available"
printf 'int pcfunc(void) { return 1; }\n' >x.c
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: no x.o"
[ ! -e nonexistent.o ] || pc_unresolved "setup: nonexistent.o exists"
pc_test
nm nonexistent.o >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "nm of a nonexistent file gave status 0"
[ -s err ] || pc_fail "no diagnostic on standard error for a nonexistent file"
nm -P nonexistent.o x.o >o1 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although the first operand does not exist"
[ -s err ] || pc_fail "no diagnostic on standard error (missing operand first)"
awk '$1 ~ /pcfunc$/ && $2 == "T"' o1 | grep . >/dev/null || pc_fail "the operand after the missing one was not listed"
nm -P x.o nonexistent.o >o2 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 although the last operand does not exist"
[ -s err ] || pc_fail "no diagnostic on standard error (missing operand last)"
awk '$1 ~ /pcfunc$/ && $2 == "T"' o2 | grep . >/dev/null || pc_fail "the operand before the missing one was not listed"
pc_pass
