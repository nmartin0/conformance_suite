# awk leaves a seekable standard input positioned just past the last record it processed when it exits before end-of-file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.013
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
nl='
'
printf 'l1\nl2\nl3\nl4\nl5\n' >f || pc_unresolved "cannot write the input"
pc_test
got=$( (awk 'NR == 2 { exit }' >/dev/null; cat) <f )
[ "$got" = "l3${nl}l4${nl}l5" ] || pc_fail "(awk 'NR == 2 { exit }'; cat): got '$got'"
pc_pass
