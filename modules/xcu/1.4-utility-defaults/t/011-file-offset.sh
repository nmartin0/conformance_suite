# When a standard utility reads a seekable input file and ends without error before end-of-file, the file offset is left just past the last byte it processed, so (sed -n 1q; cat) gives what tail -n +2 gives; sed 2q, head -n 2 and dd bs=3 count=1 behave likewise
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.011
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
nl='
'
printf 'l1\nl2\nl3\nl4\nl5\n' >f || pc_unresolved "cannot write the input"
pc_test
got=$( (sed -n 1q; cat) <f )
want=$(tail -n +2 f)
[ "$got" = "$want" ] || pc_fail "(sed -n 1q; cat): got '$got'"
[ "$got" = "l2${nl}l3${nl}l4${nl}l5" ] || pc_fail "(sed -n 1q; cat): unexpected output '$got'"
got=$( (sed 2q >/dev/null; cat) <f )
[ "$got" = "l3${nl}l4${nl}l5" ] || pc_fail "(sed 2q; cat): got '$got'"
got=$( (head -n 2 >/dev/null; cat) <f )
[ "$got" = "l3${nl}l4${nl}l5" ] || pc_fail "(head -n 2; cat): got '$got'"
got=$( (dd bs=3 count=1 of=/dev/null 2>/dev/null; cat) <f )
[ "$got" = "l2${nl}l3${nl}l4${nl}l5" ] || pc_fail "(dd bs=3 count=1; cat): got '$got'"
pc_pass
