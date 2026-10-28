# When sh reads commands from standard input and runs a command that also
# reads standard input, the input is left just after the command line that
# started it: the shell has not read ahead.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.017
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.017
pc_test
nl='
'
r=$(printf 'cat\nA\nB\n' | ${PC_SH:-sh})
[ "$r" = "A${nl}B" ] || pc_fail "cat got '$r', expected A and B"
r=$(printf 'read x\nVALUE\necho "got $x"\n' | ${PC_SH:-sh})
[ "$r" = "got VALUE" ] || pc_fail "read got '$r'"
pc_pass
