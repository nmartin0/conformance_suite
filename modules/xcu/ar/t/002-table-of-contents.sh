# ar -t lists member names
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.002
# "ar -t a" prints the member names one per line in archive order;
# with operands only those members are listed.
# MUTANT    ar.t_reverse
# MUTANT    ar.t_sorted
# MUTANT    ar.t_operands_ignored
# MUTANT    ar.t_no_final_newline
# MUTANT    ar.t_status1
# MUTANT    ar.t_first_operand_only
# MUTANT    ar.t_extra_blank
# MUTANT    ar.t_to_stderr
# MUTANT    ar.t_first_only
# MUTANT    ar.t_drops_last
# VARIANT   ar.operand_reverse
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
printf 'one\n' >f1; printf 'two\n' >f2; printf 'three\n' >f3
# Archive order deliberately differs from name order.
pc_mk a f3 f1 f2 || pc_unresolved "setup: cannot build the archive"
printf 'f3\nf1\nf2\n' >exp.all
printf 'f2\n' >exp.f2
printf 'f1\nf3\n' >exp.f1f3
pc_test
ar -t a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -t a: exit status $rc"
[ -f out ] || pc_fail "no output file"
cmp -s out exp.all || pc_fail "ar -t listed '$(cat out)', expected the names in archive order"
ar -t a f2 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -t a f2: exit status $rc"
[ -f out ] || pc_fail "no output file"
cmp -s out exp.f2 || pc_fail "ar -t a f2 listed '$(cat out)'"
ar -t a f1 >out 2>err </dev/null || pc_fail "ar -t a f1: nonzero status"
[ "$(cat out)" = f1 ] || pc_fail "ar -t a f1 listed '$(cat out)'"
# With several operands only those are listed; the order of the lines is
# not fixed by the text, so compare them sorted.
ar -t a f3 f1 >out 2>err </dev/null || pc_fail "ar -t a f3 f1: nonzero status"
[ -f out ] || pc_fail "no output file"
sort out >out.s
cmp -s out.s exp.f1f3 || pc_fail "ar -t a f3 f1 listed '$(cat out)', expected just those two"
pc_pass
