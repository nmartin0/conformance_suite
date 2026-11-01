# rescanning and non-macro tokens
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.013
# A macro's defining text is scanned together with the input that follows it,
# and a token that is not a macro name, even if it contains one, is copied.
# MUTANT    m4.no_rescan
# MUTANT    m4.rescan_isolated
# MUTANT    m4.name_no_underscore
# MUTANT    m4.name_no_digit
# MUTANT    m4.name_shortest
# MUTANT    m4.name_digit_start
# VARIANT   m4.engine
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in <<'EOM'
define(`f',`g(')define(`g',`[$1]')f`'x)
define(`ab',`Z')abc ab_ xab (ab) ab ab1 1ab
define(`xa',`XA')define(`h',`x')h()a
EOM
printf '[x]\nabc ab_ xab (Z) Z ab1 1Z\nXA\n' >exp
pc_test
m4 in >out 2>err </dev/null || pc_fail "m4 failed"
cmp -s out exp || pc_fail "output was '$(cat out)', expected '$(cat exp)'"
pc_pass
