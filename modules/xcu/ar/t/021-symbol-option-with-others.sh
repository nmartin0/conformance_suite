# -s with -t, -p and -x leaves their results unchanged
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.021
# "ar -ts a" lists the members as "ar -t a" does, "ar -ps a" writes
# their contents as "ar -p a" does, and "ar -xs a" extracts them as
# "ar -x a" does.
# MUTANT    ar.ts_silent
# MUTANT    ar.ps_silent
# MUTANT    ar.xs_silent
# MUTANT    ar.ts_status1
# MUTANT    ar.ts_reverse
# MUTANT    ar.ps_reverse
# MUTANT    ar.xs_first_only
# MUTANT    ar.t_s_separate_silent
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.021
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
printf 'one\n' >f1; printf 'two\n' >f2; printf 'odd' >f3
pc_mk a f2 f1 f3 || pc_unresolved "setup: cannot build the archive"
cp f1 f1.keep; cp f2 f2.keep; cp f3 f3.keep
cat f2 f1 f3 >all.exp
printf 'f2\nf1\nf3\n' >list.exp
pc_test
ar -ts a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -ts: exit status $rc"
[ -f out ] || pc_fail "no output file"
cmp -s out list.exp || pc_fail "ar -ts listed '$(cat out)'"
ar -t -s a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -t -s: exit status $rc"
[ -f out ] || pc_fail "no output file"
cmp -s out list.exp || pc_fail "ar -t -s listed '$(cat out)'"
ar -ps a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -ps: exit status $rc"
[ -f out ] || pc_fail "no output file"
cmp -s out all.exp || pc_fail "ar -ps did not write the contents of the members in order"
rm -f f1 f2 f3
ar -xs a >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ar -xs: exit status $rc"
for f in f1 f2 f3; do
	[ -f "$f" ] || pc_fail "ar -xs did not extract $f"
	cmp -s "$f" "$f.keep" || pc_fail "ar -xs extracted the wrong contents for $f"
done
pc_pass
