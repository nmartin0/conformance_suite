# With -c the shell reads no commands from standard input, and the standard
# input stays available to the commands it runs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.003
pc_test
r=$(printf 'echo from-stdin\n' | ${PC_SH:-sh} -c 'echo from-c')
[ "$r" = from-c ] || pc_fail "got '$r', expected only from-c"
r=$(printf 'data\n' | ${PC_SH:-sh} -c 'read x; echo "$x"')
[ "$r" = data ] || pc_fail "the command did not get the standard input (got '$r')"
pc_pass
