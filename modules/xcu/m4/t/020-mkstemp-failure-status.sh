# mkstemp that cannot create a file: the final status
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.020
# When mkstemp cannot create its file the final exit status is non-zero, even
# when m4exit is later given a zero argument.
# MUTANT    m4.mkstemp_fail_status0
# MUTANT    m4.err_forgotten
# MUTANT    m4.m4exit_ignores_mkstemp_err
# VARIANT   m4.engine
# VARIANT   m4.err_status2
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.020
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a[mkstemp(`%s/nodir/xXXXXXX'"'"')]b\nend\n' "$PWD" >in
printf 'a[mkstemp(`%s/nodir/xXXXXXX'"'"')]b\nm4exit(0)\n' "$PWD" >in2
pc_test
m4 in >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "exit status 0 although a file could not be created"
m4 in2 >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "m4exit(0) after a failed mkstemp gave status 0"
pc_pass
