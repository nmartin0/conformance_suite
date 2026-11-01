# -x with -n and -s.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.009
# MUTANT    xargs.x_ignored
# MUTANT    xargs.x_runs_anyway
# MUTANT    xargs.x_status0
# MUTANT    xargs.x_status126
# MUTANT    xargs.x_no_diag
# MUTANT    xargs.x_always_error
# MUTANT    xargs.x_last_error
# MUTANT    xargs.nox_fail
# MUTANT    xargs.nox_drops
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.009
LC_ALL=C; export LC_ALL
nl='
'
pc_test
# echo + one argument is 10 bytes, with two arguments 15: size 12 holds one.
printf 'aaaa bbbb\n' | xargs -n 2 -s 12 echo >out 2>/dev/null || pc_fail "without -x, xargs -n 2 -s 12 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "aaaa${nl}bbbb$nl" ] || pc_fail "without -x the output was '$v', expected 'aaaa' and 'bbbb' on separate lines"
printf 'aaaa bbbb\n' | xargs -x -n 2 -s 12 echo >out 2>err
rc=$?
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "with -x the status was $rc, expected 1-125"
v=$(cat out; printf x)
v=${v%x}
case $v in
*aaaa*bbbb*) pc_fail "with -x a command line that did not fit was run: '$v'" ;;
esac
[ -s err ] || pc_fail "xargs -x wrote no diagnostic message for a command line that did not fit"
# when the lines fit, -x changes nothing; a short last group is not an error
printf 'aaaa bbbb\n' | xargs -x -n 2 -s 100 echo >out 2>/dev/null || pc_fail "xargs -x -n 2 -s 100 failed although everything fits"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "aaaa bbbb$nl" ] || pc_fail "with -x and a large size the output was '$v'"
printf 'a b c\n' | xargs -x -n 2 echo >out 2>/dev/null || pc_fail "xargs -x -n 2 failed on a short last group"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "a b${nl}c$nl" ] || pc_fail "with -x -n 2 the output was '$v', expected 'a b' and 'c'"
pc_pass
