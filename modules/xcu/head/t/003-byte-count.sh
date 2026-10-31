# head -c number writes exactly that many bytes, adding nothing, whether or not that ends a line.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.head.003
# MUTANT    head.c_less
# MUTANT    head.c_more
# MUTANT    head.c_ignored
# MUTANT    head.c_as_lines
# MUTANT    head.c_to_eol
# MUTANT    head.c_adds_newline
# MUTANT    head.c_cap4096
# MUTANT    head.c_nul_dropped
# MUTANT    head.c_high_altered
# MUTANT    head.short_status1
# MUTANT    head.short_nothing
. "$PC_CORE/pc.sh"
pc_begin XCU.head.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij\nklm\n' >in || pc_fail "cannot create input"
printf 'a\000b\377c\n' >bin || pc_fail "cannot create input"
printf 'a\000b\377' >binexp || pc_fail "cannot create input"
l=0123456789
i=0
while [ $i -lt 10 ]; do l=$l$l; i=$((i + 1)); done
printf '%s' "$l" >blk || pc_fail "cannot create input"
pc_test
got=$(head -c 5 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcderc=0" ] || pc_fail "-c 5 did not write exactly five bytes"
got=$(head -c 12 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}krc=0" ] || pc_fail "-c 12 did not write exactly twelve bytes"
got=$(head -c 100 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}klm${NL}rc=0" ] || pc_fail "-c 100 did not copy the whole file with status 0"
head -c 4 bin >binout 2>/dev/null </dev/null || pc_fail "-c 4 failed on binary data"
cmp -s binout binexp || pc_fail "NUL or high-bit bytes were not copied intact"
head -c 5000 blk >blkout 2>/dev/null </dev/null || pc_fail "-c 5000 failed"
[ "$(wc -c <blkout)" -eq 5000 ] || pc_fail "-c 5000 did not write exactly 5000 bytes"
head -c 5000 blk | cmp -s - blkout || pc_fail "-c 5000 gave different output on a second run"
pc_pass
