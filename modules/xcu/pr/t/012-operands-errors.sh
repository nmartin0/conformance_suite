# pr reads standard input without an operand or with -, handles several files, and reports unopenable files unless -r.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pr.012
. "$PC_CORE/pc.sh"
pc_begin XCU.pr.012
LC_ALL=POSIX
export LC_ALL
NL='
'
mkn() { _n=$1; _f=$2; : >"$_f" || pc_unresolved "cannot create input"; _i=1; while [ "$_i" -le "$_n" ]; do printf 'L%s\n' "$_i"; _i=$((_i+1)); done >"$_f"; }
rng() { _a=$1; _b=$2; rng_out=; while [ "$_a" -le "$_b" ]; do rng_out=$rng_out"L$_a$NL"; _a=$((_a+1)); done; }
printf 'a\n' >f1 || pc_unresolved "cannot create input"
printf 'b\n' >f2 || pc_unresolved "cannot create input"
pc_test
got=$(printf 'in\n' | pr -t 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "in${NL}rc=0" ] || pc_fail "no operand did not read standard input"
got=$(printf 'in\n' | pr -t - 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "in${NL}rc=0" ] || pc_fail "operand - did not read standard input"
got=$(pr -t f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}rc=0" ] || pc_fail "two files were not written in turn"
got=$(pr -l 15 f1 f2 2>/dev/null </dev/null; printf x)
case $got in *" f1 Page "*"a$NL"*" f2 Page "*"b$NL"*) ;; *) pc_fail "each file does not have a header with its own name";; esac
pr -t nonexistent-file </dev/null >/dev/null 2>pr.err
rc=$?
[ "$rc" -gt 0 ] || pc_fail "a missing file gave status 0"
[ -s pr.err ] || pc_fail "a missing file gave no diagnostic"
got=$(pr -t -r nonexistent-file f1 2>pr.err2 </dev/null)
[ "$got" = a ] || pc_fail "-r did not keep the other file's output"
[ ! -s pr.err2 ] || pc_fail "-r did not suppress the diagnostic"
pc_pass
