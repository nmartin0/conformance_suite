# uudecode -o overrides the name in the data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.002
# With -o outfile the decoded bytes go to outfile and no file with the name
# from the data is created.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin 644 out.bin\n#0V%%T\n`\nend\n' >in || pc_unresolved "setup: cannot write input"
pc_test
uudecode -o other.bin in >stdout 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
[ -f other.bin ] || pc_fail "the -o file was not created"
[ "$(cat other.bin)" = Cat ] || pc_fail "decoded content is wrong"
[ -e out.bin ] && pc_fail "the name from the data was used as well"
pc_pass
