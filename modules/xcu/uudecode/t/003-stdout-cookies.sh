# uudecode to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.003
# A name of /dev/stdout, whether given by the data or by -o, sends the
# decoded bytes to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin 644 /dev/stdout\n#0V%%T\n`\nend\n' >in2 || pc_unresolved "setup: cannot write input"
printf 'begin 644 out.bin\n#0V%%T\n`\nend\n' >in3 || pc_unresolved "setup: cannot write input"
pc_test
uudecode in2 >o2 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "name /dev/stdout: exit status $rc"
[ "$(cat o2)" = Cat ] || pc_fail "name /dev/stdout: standard output is wrong"
uudecode -o /dev/stdout in3 >o4 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-o /dev/stdout: exit status $rc"
[ "$(cat o4)" = Cat ] || pc_fail "-o /dev/stdout: standard output is wrong"
[ -e out.bin ] && pc_fail "-o /dev/stdout still created the file from the data"
pc_pass
