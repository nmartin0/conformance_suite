# Variables of the standard utilities' own languages start at zero when the input does not assign them: an awk variable or array element, a bc variable or array element, and an unset shell name in arithmetic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.1-relationship.013
. "$PC_CORE/pc.sh"
pc_begin XCU.1.1-relationship.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
nl='
'
pc_test
got=$(awk 'BEGIN { print x + 0; print a["k"] + 0; print (x == 0), (x == "") }') || pc_fail "awk failed"
[ "$got" = "0${nl}0${nl}1 1" ] || pc_fail "awk: got '$got'"
got=$(printf 'x + 1\na[3] + 2\n' | bc) || pc_fail "bc failed"
[ "$got" = "1${nl}2" ] || pc_fail "bc: got '$got'"
got=$(unset pcz; echo $((pcz + 5)))
[ "$got" = 5 ] || pc_fail "sh: an unset name in arithmetic gave '$got'"
pc_pass
