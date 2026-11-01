# uudecode reads standard input when no file is given
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.005
# MUTANT    uudecode.stdin_none
# MUTANT    uudecode.status1
# MUTANT    uudecode.drop_last
# MUTANT    uudecode.no_create
# With no file operand the encoded data is read from standard input.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin 644 s.out\n#0V%%T\n`\nend\n' >in || pc_unresolved "setup: cannot write input"
printf Cat >exp || pc_unresolved "setup: cannot write expectation"
pc_test
uudecode <in >stdout 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
[ -f s.out ] || pc_fail "s.out was not created"
cmp -s exp s.out 2>/dev/null || pc_fail "decoded content is wrong"
pc_pass
