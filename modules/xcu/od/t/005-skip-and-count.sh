# od -j skips bytes (decimal, octal or hexadecimal) and fails if the input is shorter; -N limits the bytes formatted and is no error if fewer remain.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.005
. "$PC_CORE/pc.sh"
pc_begin XCU.od.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij' >f1 || pc_fail "cannot create input"
pc_test
got=$(od -A n -v -t x1 -j 2 -N 3 f1 2>/dev/null </dev/null)
set -- $got
[ "$*" = "63 64 65" ] || pc_fail "-j 2 -N 3 is wrong"
got=$(od -A n -v -t x1 -j 010 f1 2>/dev/null </dev/null)
set -- $got
[ "$*" = "69 6a" ] || [ "$*" = "69 6A" ] || pc_fail "-j 010 did not skip eight bytes"
got=$(od -A n -v -t x1 -j 0x4 -N 0x2 f1 2>/dev/null </dev/null)
set -- $got
[ "$*" = "65 66" ] || pc_fail "-j 0x4 -N 0x2 is wrong"
got=$(od -A n -v -t x1 -N 100 f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $got in
*rc=0) ;;
*) pc_fail "-N larger than the input was an error" ;;
esac
set -- ${got%rc=0}
[ $# -eq 10 ] || pc_fail "-N larger than the input did not format all ten bytes"
od -A n -v -t x1 -j 100 f1 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-j beyond the end of the input gave status 0"
pc_pass
