# head -c number writes exactly that many bytes, adding nothing, whether or not that ends a line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.003
. "$PC_CORE/pc.sh"
pc_begin XCU.head.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij\nklm\n' >in || pc_fail "cannot create input"
pc_test
got=$(head -c 5 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcderc=0" ] || pc_fail "-c 5 did not write exactly five bytes"
got=$(head -c 12 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}krc=0" ] || pc_fail "-c 12 did not write exactly twelve bytes"
got=$(head -c 100 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}klm${NL}rc=0" ] || pc_fail "-c 100 did not copy the whole file with status 0"
pc_pass
