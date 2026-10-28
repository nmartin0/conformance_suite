# od reads its file operands in order as one stream, and standard input when there is no operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.008
. "$PC_CORE/pc.sh"
pc_begin XCU.od.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc' >f1 && printf 'def' >f2 || pc_fail "cannot create input"
[ ! -e nosuchfile ] || pc_unresolved "nosuchfile exists"
pc_test
got=$(od -A n -v -t x1 f1 f2 2>/dev/null </dev/null)
set -- $got
[ "$*" = "61 62 63 64 65 66" ] || pc_fail "two files were not read as one stream"
got=$(od -A n -v -t x1 <f2 2>/dev/null)
set -- $got
[ "$*" = "64 65 66" ] || pc_fail "standard input was not read"
od -A n -v -t x1 nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 for a file that cannot be read"
od -A d -v -t x1 f1 f2 >out 2>/dev/null </dev/null
last=
while IFS= read -r line; do last=$line; done <out
set -- $last
z=${1%%[!0]*}
off=${1#"$z"}
[ $# -eq 1 ] && [ "$off" = 6 ] || pc_fail "the final offset does not count the bytes of both files"
pc_pass
