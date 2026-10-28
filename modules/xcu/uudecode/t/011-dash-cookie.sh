# uudecode: - means standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.011
# A name of -, whether given by the data or by -o, sends the decoded bytes
# to standard output and creates no file of that name.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin 644 -\n#0V%%T\n`\nend\n' >in1 || pc_unresolved "setup: cannot write input"
printf 'begin 644 out.bin\n#0V%%T\n`\nend\n' >in3 || pc_unresolved "setup: cannot write input"
pc_test
uudecode in1 >o1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "name '-': exit status $rc"
[ "$(cat o1)" = Cat ] || pc_fail "name '-': standard output is wrong"
[ -e ./- ] && pc_fail "a file named - was created"
uudecode -o - in3 >o3 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-o -: exit status $rc"
[ "$(cat o3)" = Cat ] || pc_fail "-o -: standard output is wrong"
[ -e ./- ] && pc_fail "-o - created a file named -"
[ -e out.bin ] && pc_fail "-o - still created the file from the data"
pc_pass
