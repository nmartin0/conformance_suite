# l command format
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.015
# l shows a tab as \t, a backslash as \\, a dollar inside the text as \$, a
# control byte as a three-digit octal escape, and ends each line with $.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a\tb\nc$d\ne\\f\ng\001h\n' >f || pc_unresolved "setup: cannot write f"
printf '1,4l\nQ\n' >s
printf 'a\\tb$\nc\\$d$\ne\\\\f$\ng\\001h$\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "l output was: $(cat o)"
pc_pass
