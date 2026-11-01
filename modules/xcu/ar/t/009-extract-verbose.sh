# ar -xv output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.009
# "ar -xv a f1" writes "x - f1" to standard output.
# MUTANT    ar.xv_no_output
# MUTANT    ar.xv_no_dash
# MUTANT    ar.xv_letter
# MUTANT    ar.xv_stderr
# MUTANT    ar.xv_no_newline
# MUTANT    ar.xv_trailing_space
# MUTANT    ar.xv_no_extract
# MUTANT    ar.xv_operand_all
# MUTANT    ar.xv_status1
# MUTANT    ar.xv_names_all
# VARIANT   ar.operand_reverse
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
printf 'one\n' >f1; printf 'two\n' >f2
pc_mk a f2 f1 || pc_unresolved "setup: cannot build the archive"
rm -f f1 f2
pc_test
ar -xv a f1 >out 2>err </dev/null || pc_fail "ar -xv a f1: nonzero status"
[ -f out ] || pc_fail "no output file"
printf 'x - f1\n' >exp
cmp -s out exp || pc_fail "ar -xv a f1 wrote '$(cat out)'"
[ -f f1 ] || pc_fail "ar -xv a f1 did not extract f1"
[ -e f2 ] && pc_fail "ar -xv a f1 extracted f2"
rm -f f1
ar -xv a >out 2>err </dev/null || pc_fail "ar -xv a: nonzero status"
[ -f out ] || pc_fail "no output file"
# The order of the lines is not fixed by the text.
sort out >out.s
printf 'x - f1\nx - f2\n' >exp
cmp -s out.s exp || pc_fail "ar -xv a wrote '$(cat out)'"
[ -f f1 ] && [ -f f2 ] || pc_fail "ar -xv a did not extract the members"
rm -f f1 f2
# Several operands: the order of the lines is not fixed.
ar -xv a f1 f2 >out 2>err </dev/null || pc_fail "ar -xv a f1 f2: nonzero status"
[ -f out ] || pc_fail "no output file"
sort out >out.s
printf 'x - f1\nx - f2\n' >exp
cmp -s out.s exp || pc_fail "ar -xv a f1 f2 wrote '$(cat out)'"
pc_pass
