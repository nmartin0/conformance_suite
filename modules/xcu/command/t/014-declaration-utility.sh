# command export is expanded as an assignment: tilde, no splitting, no globbing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.command.014
# MUTANT    command.nondeclaration
#
# The first argument is export, a declaration utility, so the words of the
# form name=word that follow are expanded in an assignment context (tilde
# expansion after the equals sign and after an unquoted colon; no field
# splitting and no pathname expansion of the results).
. "$PC_CORE/pc.sh"
pc_begin XCU.command.014
HOME=/pc/home
two='a b'
pat='o*'
: >'pcg=one' || pc_fail "cannot make a file"
unset pcx pcy pcz pcg
pc_test
command export pcx=~/d pcy=$two pcz=~/a:~/b pcg=$pat 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "command export exited $rc"
[ "${pcx-}" = /pc/home/d ] || pc_fail "tilde after the equals sign gave '${pcx-}', wanted /pc/home/d"
[ "${pcz-}" = /pc/home/a:/pc/home/b ] || pc_fail "tilde after a colon gave '${pcz-}', wanted /pc/home/a:/pc/home/b"
[ "${pcy-}" = 'a b' ] || pc_fail "the result of a parameter expansion was split: got '${pcy-}', wanted 'a b'"
[ "${pcg-}" = 'o*' ] || pc_fail "the result of a parameter expansion was used as a pattern: got '${pcg-}', wanted 'o*'"
pc_pass
