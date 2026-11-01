# od reads its file operands in order as one stream, and standard input when there is no operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.008
# MUTANT    od.files_reversed
# MUTANT    od.first_only
# MUTANT    od.last_only
# MUTANT    od.stdin_ignored
# MUTANT    od.reads_stdin_too
# MUTANT    od.per_file_offsets
# MUTANT    od.status0_missing
# MUTANT    od.status0_partial
# VARIANT   od.status2
# VARIANT   od.status255
# VARIANT   od.off_wide
# VARIANT   od.gap3
# VARIANT   od.an_empty
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
# with a file operand standard input is not read
got=$(od -A n -v -t x1 f1 <f2 2>/dev/null)
set -- $got
[ "$*" = "61 62 63" ] || pc_fail "standard input was read although there is a file operand"
# an unreadable file anywhere among the operands is an error
od -A n -v -t x1 f1 nosuchfile f2 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 when one of several files cannot be read"
od -A n -v -t x1 f1 f2 nosuchfile >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "status 0 when the last of several files cannot be read"
od -A n -v -t x1 f1 f2 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "status $rc when every file can be read"
pc_pass
