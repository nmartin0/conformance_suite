# wc -l, -w and -c each write just that count and the file name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.002
# MUTANT    wc.l_all
# MUTANT    wc.w_all
# MUTANT    wc.c_all
# MUTANT    wc.opt_extra_l
# MUTANT    wc.opt_extra_w
# MUTANT    wc.opt_extra_c
# MUTANT    wc.l_ignored
# MUTANT    wc.w_ignored
# MUTANT    wc.c_ignored
# MUTANT    wc.l_off
# MUTANT    wc.w_off
# MUTANT    wc.c_off
# MUTANT    wc.c_nul_stop
# MUTANT    wc.c_high_dropped
# MUTANT    wc.name_missing
# MUTANT    wc.status1
# MUTANT    wc.wrap16
# MUTANT    wc.cap6
# VARIANT   wc.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
printf 'a\0b\n\377\200c\n' >bin || pc_fail "cannot create input"
awk 'BEGIN { for (i = 0; i < 400000; i++) print "a b" }' >big || pc_fail "cannot create input"
pc_test
out=$(wc -l f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 2 ] && [ "$1" = 4 ] && [ "$2" = f ] || pc_fail "wc -l f is not '4 f'"
out=$(wc -w f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 2 ] && [ "$1" = 6 ] && [ "$2" = f ] || pc_fail "wc -w f is not '6 f'"
out=$(wc -c f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 2 ] && [ "$1" = 29 ] && [ "$2" = f ] || pc_fail "wc -c f is not '29 f'"
# bytes of any value are counted, a null byte and bytes with the high bit set included
out=$(wc -c bin 2>/dev/null </dev/null)
set -- $out
[ $# -eq 2 ] && [ "$1" = 8 ] && [ "$2" = bin ] || pc_fail "wc -c bin is not '8 bin'"
out=$(wc -l bin 2>/dev/null </dev/null)
set -- $out
[ $# -eq 2 ] && [ "$1" = 2 ] && [ "$2" = bin ] || pc_fail "wc -l bin is not '2 bin'"
# large counts
out=$(wc -l big 2>/dev/null </dev/null)
set -- $out
[ $# -eq 2 ] && [ "$1" = 400000 ] && [ "$2" = big ] || pc_fail "wc -l big is not '400000 big'"
out=$(wc -w big 2>/dev/null </dev/null)
set -- $out
[ $# -eq 2 ] && [ "$1" = 800000 ] && [ "$2" = big ] || pc_fail "wc -w big is not '800000 big'"
out=$(wc -c big 2>/dev/null </dev/null)
set -- $out
[ $# -eq 2 ] && [ "$1" = 1600000 ] && [ "$2" = big ] || pc_fail "wc -c big is not '1600000 big'"
pc_pass
