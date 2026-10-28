# syscmd, sysval, m4exit, errprint
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.010
# syscmd runs a command and sysval gives its exit value; m4exit sets the
# exit status and stops processing; errprint writes to standard error only.
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
pc_test
m4 in2 >out 2>err </dev/null || pc_fail "syscmd: nonzero status"
[ "$(cat out)" = 3 ] || pc_fail "sysval after exit 3 gave '$(cat out)'"
m4 in >out 2>err </dev/null
grep hi out >/dev/null || pc_fail "syscmd output 'hi' is missing"
m4 in3 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 5 ] || pc_fail "m4exit(5): status $rc"
grep after out >/dev/null && pc_fail "text after m4exit was processed"
m4 in4 >out 2>err </dev/null
grep oops err >/dev/null || pc_fail "errprint wrote nothing to standard error"
grep oops out >/dev/null && pc_fail "errprint wrote to standard output"
[ "$(cat out)" = visible ] || pc_fail "standard output was '$(cat out)'"
pc_pass
