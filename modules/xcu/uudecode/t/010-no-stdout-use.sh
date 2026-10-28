# uudecode writes nothing to standard output when decoding to a file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.010
# When the output is a named file, nothing is written to standard output.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin 644 q.out\n#0V%%T\n`\nend\n' >in || pc_unresolved "setup: cannot write input"
pc_test
uudecode in >stdout 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
[ -s stdout ] && pc_fail "decoding to a file wrote to standard output"
pc_pass
