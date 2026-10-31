# tail -c number starts relative to the end of the file in bytes, or from the numberth byte when the number has a plus sign.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tail.003
# MUTANT    tail.c_ignored
# MUTANT    tail.c_less
# MUTANT    tail.c_more
# MUTANT    tail.c_plus_off_late
# MUTANT    tail.c_plus_off_early
# MUTANT    tail.c_plus_as_minus
# MUTANT    tail.c_minus_as_plus
# MUTANT    tail.c_as_lines
# MUTANT    tail.c_zero_default
# MUTANT    tail.c_big_nothing
# MUTANT    tail.c_nul_dropped
# MUTANT    tail.c_high_altered
# MUTANT    tail.large_first_block
# MUTANT    tail.out_extra_newline
# MUTANT    tail.out_no_final_newline
# VARIANT   tail.byte_writes
# VARIANT   tail.buffer_limit
. "$PC_CORE/pc.sh"
pc_begin XCU.tail.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij\n' >in || pc_fail "cannot create input"
# arbitrary data: a NUL byte, bytes with the high bit set, no final newline
printf 'ab\000cd\377\200ef\001' >bin || pc_fail "cannot create input"
printf '\000cd\377\200ef\001' >bin8 || pc_fail "cannot create input"
printf '\377\200ef\001' >bin4 || pc_fail "cannot create input"
printf 'ef\001' >bin3p || pc_fail "cannot create input"
printf 'abc' >nonl || pc_fail "cannot create input"
printf 'bc' >nonl2 || pc_fail "cannot create input"
awk 'BEGIN { for (i = 1; i <= 20000; i++) print "line " i }' >big || pc_fail "cannot create input"
printf '999\nline 20000\n' >bigtail || pc_fail "cannot create input"
printf '20000\n' >bigplus || pc_fail "cannot create input"
sz=$(wc -c <big)
pc_test
got=$(tail -c 4 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "hij${NL}rc=0" ] || pc_fail "-c 4 did not write the last four bytes"
got=$(tail -c -4 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "hij${NL}rc=0" ] || pc_fail "-c -4 did not write the last four bytes"
got=$(tail -c +9 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ij${NL}rc=0" ] || pc_fail "-c +9 did not start at the ninth byte"
got=$(tail -c +1 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}rc=0" ] || pc_fail "-c +1 did not write the whole file"
got=$(tail -c +11 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}rc=0" ] || pc_fail "-c +11 did not start at the last byte"
got=$(tail -c +12 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-c +12 on an eleven byte file did not write nothing"
got=$(tail -c 1 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}rc=0" ] || pc_fail "-c 1 did not write the last byte"
got=$(tail -c -1 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${NL}rc=0" ] || pc_fail "-c -1 did not write the last byte"
got=$(tail -c 0 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-c 0 did not write nothing"
got=$(tail -c 11 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}rc=0" ] || pc_fail "-c 11 on an eleven byte file did not write it whole"
got=$(tail -c 100 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abcdefghij${NL}rc=0" ] || pc_fail "-c 100 on an eleven byte file did not write it whole"
tail -c 2 nonl >out 2>/dev/null </dev/null || pc_fail "-c 2 on a file without a final newline failed"
cmp -s out nonl2 || pc_fail "-c 2 on a file without a final newline did not write exactly its last two bytes"
tail -c 8 bin >out 2>/dev/null </dev/null || pc_fail "-c 8 on binary data failed"
cmp -s out bin8 || pc_fail "-c 8 on binary data did not write the bytes unchanged"
tail -c +6 bin >out 2>/dev/null </dev/null || pc_fail "-c +6 on binary data failed"
cmp -s out bin4 || pc_fail "-c +6 on binary data did not write the bytes unchanged"
tail -c 3 bin >out 2>/dev/null </dev/null || pc_fail "-c 3 on binary data failed"
cmp -s out bin3p || pc_fail "-c 3 on binary data did not write the bytes unchanged"
# a file much larger than any single block of input
tail -c 15 big >out 2>/dev/null </dev/null || pc_fail "-c 15 on a large file failed"
cmp -s out bigtail || pc_fail "-c 15 on a large file did not write its last fifteen bytes"
tail -c +$((sz - 5)) big >out 2>/dev/null </dev/null || pc_fail "-c +N on a large file failed"
cmp -s out bigplus || pc_fail "-c +N on a large file did not write its last six bytes"
tail -c +1 big >out 2>/dev/null </dev/null || pc_fail "-c +1 on a large file failed"
cmp -s out big || pc_fail "-c +1 on a large file did not copy it whole"
pc_pass
