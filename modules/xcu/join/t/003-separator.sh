# join -t makes the character the only separator of input and output fields, so empty fields count.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.join.003
. "$PC_CORE/pc.sh"
pc_begin XCU.join.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a:1:\nb::3\nc d:4\n' >f1 && printf 'a:x\nb:y\nc d:z\n' >f2 || pc_fail "cannot create input"
pc_test
got=$(join -t : f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a:1::x${NL}b::3:y${NL}c d:4:z${NL}rc=0" ] || pc_fail "-t : gave the wrong result"
pc_pass
