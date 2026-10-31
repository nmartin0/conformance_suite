# Whatever the order of the options, wc writes lines, then words, then bytes, and only those asked for.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.wc.004
# MUTANT    wc.order_args
# MUTANT    wc.opt_last_wins
# MUTANT    wc.swap_lw
# MUTANT    wc.drop_l_combo
# MUTANT    wc.opt_extra_l
# MUTANT    wc.opt_extra_w
# MUTANT    wc.opt_extra_c
# MUTANT    wc.l_ignored
# MUTANT    wc.w_ignored
# MUTANT    wc.c_ignored
# VARIANT   wc.padded
. "$PC_CORE/pc.sh"
pc_begin XCU.wc.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one two\nthree\n\nfour five six\n' >f || pc_fail "cannot create input"
pc_test
out=$(wc -wl f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 3 ] && [ "$1" = 4 ] && [ "$2" = 6 ] && [ "$3" = f ] || pc_fail "wc -wl f is not '4 6 f'"
out=$(wc -cw f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 3 ] && [ "$1" = 6 ] && [ "$2" = 29 ] && [ "$3" = f ] || pc_fail "wc -cw f is not '6 29 f'"
out=$(wc -cl f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 3 ] && [ "$1" = 4 ] && [ "$2" = 29 ] && [ "$3" = f ] || pc_fail "wc -cl f is not '4 29 f'"
out=$(wc -lc -w f 2>/dev/null </dev/null)
rc=$?
set -- $out
[ "$rc" -eq 0 ] && [ $# -eq 4 ] && [ "$1" = 4 ] && [ "$2" = 6 ] && [ "$3" = 29 ] && [ "$4" = f ] || pc_fail "wc -lc -w f is not '4 6 29 f'"
pc_pass
