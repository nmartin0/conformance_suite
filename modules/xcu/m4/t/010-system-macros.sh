# syscmd, sysval, m4exit, errprint
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.010
# syscmd runs a command and sysval gives its exit value; m4exit sets the
# exit status and stops processing; errprint writes to standard error only.
# MUTANT    m4.syscmd_noop
# MUTANT    m4.syscmd_exec_direct
# MUTANT    m4.syscmd_stdout_err
# MUTANT    m4.sysval_zero
# MUTANT    m4.sysval_one
# MUTANT    m4.sysval_mod128
# MUTANT    m4.sysval_times256
# MUTANT    m4.sysval_sticky
# MUTANT    m4.sysval_only_first
# MUTANT    m4.m4exit_continues
# MUTANT    m4.m4exit_ignores_arg
# MUTANT    m4.m4exit_noarg_1
# MUTANT    m4.m4exit_clamp127
# MUTANT    m4.m4exit_mod16
# MUTANT    m4.m4exit_discards
# MUTANT    m4.exit_status0
# MUTANT    m4.errprint_stdout
# MUTANT    m4.errprint_both
# MUTANT    m4.errprint_nothing
# MUTANT    m4.errprint_trunc
# VARIANT   m4.engine
# VARIANT   m4.syscmd_captured
# VARIANT   m4.syscmd_noflush
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
syscmd(`echo hi')sysval
EOM
cat >in2 <<'EOM'
syscmd(`exit 3')sysval
EOM
printf 'before\nm4exit(5)after\n' >in3
cat >in4 <<'EOM'
errprint(`oops')visible
EOM
cat >in5 <<'EOM'
syscmd(`exit 3')syscmd(`exit 0')sysval
syscmd(`exit 200')sysval
EOM
cat >in6 <<'EOM'
syscmd(`echo a | tr a b; exit 4')sysval
EOM
printf 'x\nm4exit\nafter\n' >in7
printf 'x\nm4exit(0)\n' >in8
printf 'x\nm4exit(200)\n' >in9
pc_test
m4 in2 >out 2>err </dev/null || pc_fail "syscmd: nonzero status"
[ "$(cat out)" = 3 ] || pc_fail "sysval after exit 3 gave '$(cat out)'"
m4 in >out 2>err </dev/null || pc_fail "syscmd echo: nonzero status"
[ "$(cat out)" = 'hi
0' ] || pc_fail "syscmd echo hi and sysval gave '$(cat out)'"
m4 in5 >out 2>err </dev/null || pc_fail "sysval of a later command: nonzero status"
[ "$(cat out)" = '0
200' ] || pc_fail "sysval after several commands gave '$(cat out)'"
m4 in6 >out 2>err </dev/null || pc_fail "syscmd pipeline: nonzero status"
[ "$(cat out)" = 'b
4' ] || pc_fail "syscmd with a pipeline and sysval gave '$(cat out)'"
m4 in3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 5 ] || pc_fail "m4exit(5): status $rc"
grep after out >/dev/null && pc_fail "text after m4exit was processed"
[ "$(cat out)" = before ] || pc_fail "output before m4exit was '$(cat out)'"
m4 in7 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "m4exit without an argument: status $rc"
[ "$(cat out)" = x ] || pc_fail "m4exit without an argument: output '$(cat out)'"
m4 in8 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "m4exit(0): status $rc"
m4 in9 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 200 ] || pc_fail "m4exit(200): status $rc"
m4 in4 >out 2>err </dev/null || pc_fail "errprint: nonzero status"
grep oops err >/dev/null || pc_fail "errprint wrote nothing to standard error"
grep oops out >/dev/null && pc_fail "errprint wrote to standard output"
[ "$(cat out)" = visible ] || pc_fail "standard output was '$(cat out)'"
pc_pass
