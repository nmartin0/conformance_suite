# m4exit after an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.012
# m4exit with no argument or a zero argument, after an error has occurred,
# leaves the exit status non-zero.
# MUTANT    m4.exit_status0
# MUTANT    m4.err_forgotten
# MUTANT    m4.m4exit_error_forces1
# MUTANT    m4.m4exit_zero_after_error
# MUTANT    m4.m4exit_zero_arg_only
# MUTANT    m4.m4exit_noarg_only
# MUTANT    m4.m4exit_ignores_operand_err
# MUTANT    m4.m4exit_ignores_include_err
# MUTANT    m4.m4exit_noarg_1
# MUTANT    m4.m4exit_ignores_arg
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in6 <<'EOM'
include(`nonexistent')m4exit
EOM
cat >in7 <<'EOM'
include(`nonexistent')m4exit(0)
EOM
cat >in8 <<'EOM'
include(`nonexistent')m4exit(3)
EOM
printf 'x\nm4exit\n' >in9
printf 'x\nm4exit(0)\n' >in10
pc_test
m4 in6 >out 2>err </dev/null
[ "$?" -ne 0 ] || pc_fail "m4exit after an earlier error gave status 0"
m4 in7 >out 2>err </dev/null
[ "$?" -ne 0 ] || pc_fail "m4exit(0) after an earlier error gave status 0"
m4 in8 >out 2>err </dev/null
rc=$?
[ "$rc" -eq 3 ] || pc_fail "m4exit(3) after an earlier error gave status $rc"
m4 in9 >out 2>err </dev/null
[ "$?" -eq 0 ] || pc_fail "m4exit with no earlier error gave a non-zero status"
m4 in10 >out 2>err </dev/null
[ "$?" -eq 0 ] || pc_fail "m4exit(0) with no earlier error gave a non-zero status"
m4 nonexistent.pc in9 >out 2>err </dev/null
[ "$?" -ne 0 ] || pc_fail "m4exit after an operand that could not be opened gave status 0"
m4 nonexistent.pc in10 >out 2>err </dev/null
[ "$?" -ne 0 ] || pc_fail "m4exit(0) after an operand that could not be opened gave status 0"
pc_pass
