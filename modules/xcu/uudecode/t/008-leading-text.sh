# uudecode skips text before the begin line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.008
# Text lines before the begin line are skipped and the data after it is
# decoded.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'Some mail text\n\nmore text\nbegin 644 j.out\n#0V%%T\n`\nend\nafter\n' >in || pc_unresolved "setup: cannot write input"
pc_test
uudecode in >stdout 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
[ -f j.out ] || pc_fail "j.out was not created"
[ "$(cat j.out)" = Cat ] || pc_fail "decoded content is wrong"
pc_pass
