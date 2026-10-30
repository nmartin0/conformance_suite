# An interactive shell runs the file named by ENV, after parameter expansion
# of the value, in the current environment before reading its commands.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.021
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.021
# an interactive shell may keep a history file; keep it in the working directory
HISTFILE=$PWD/pc.hist
export HISTFILE
# Job control (+m) is switched off: this test is about ENV, and a shell with job
# control may wait for the terminal of whoever started the suite.
printf '%s\n' 'echo envrun' 'pcz=fromenv' >pc.envf || pc_unresolved "write"
printf '%s\n' 'exit 0' >pc.i || pc_unresolved "write"
${PC_SH:-sh} +m -i <pc.i >/dev/null 2>&1 || pc_unresolved "cannot start an interactive shell with ${PC_SH:-sh} -i"
printf '%s\n' 'echo "main $pcz"' >pc.s || pc_unresolved "write"
chmod 600 pc.envf || pc_unresolved "chmod"
pc_test
r=$(PCD="$PWD" ENV='$PCD/pc.envf' ${PC_SH:-sh} +m -i <pc.s 2>/dev/null)
nl='
'
[ "$r" = "envrun${nl}main fromenv" ] || pc_fail "got '$r'"
pc_pass
