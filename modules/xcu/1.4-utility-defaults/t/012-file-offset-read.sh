# The read utility of the shell leaves a seekable standard input positioned just past the line it consumed, and a shell reading commands from such an input leaves the rest for the commands it runs
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.012
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
nl='
'
printf 'l1\nl2\nl3\nl4\nl5\n' >f || pc_unresolved "cannot write the input"
printf 'read a\nread b\nexec cat\nl3\nl4\n' >g || pc_unresolved "cannot write the script"
pc_test
got=$( (read a; read b; cat) <f )
[ "$got" = "l3${nl}l4${nl}l5" ] || pc_fail "(read a; read b; cat): got '$got'"
got=$(${PC_SH:-sh} -c 'read a; read b; echo "$a$b"; cat' <f)
[ "$got" = "l1l2${nl}l3${nl}l4${nl}l5" ] || pc_fail "sh -c 'read ...; cat': got '$got'"
got=$(${PC_SH:-sh} <g)
[ "$got" = "l3${nl}l4" ] || pc_fail "sh reading a script from standard input: got '$got'"
pc_pass
