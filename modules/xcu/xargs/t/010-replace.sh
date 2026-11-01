# -I replacement string.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.010
# MUTANT    xargs.i_words
# MUTANT    xargs.i_no_trim
# MUTANT    xargs.i_trail_trim
# MUTANT    xargs.i_batches
# MUTANT    xargs.i_first_only
# MUTANT    xargs.i_one_arg
# MUTANT    xargs.i_args3
# MUTANT    xargs.i_fixed_braces
# MUTANT    xargs.i_cap100
# MUTANT    xargs.i_nox
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.010
LC_ALL=C; export LC_ALL
nl='
'
pc_test
printf 'a b\n  c\n' | xargs -I '{}' echo '<{}>' '{}' >out 2>/dev/null || pc_fail "xargs -I failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "<a b> a b${nl}<c> c$nl" ] || pc_fail "-I wrote '$v', expected '<a b> a b' then '<c> c'"
# replstr in several arguments (five of them, one with two instances); a different replstr
printf 'k\n' | xargs -I '{}' echo '[{}]' '<{}>' '{}{}' '({})' '{}.' >out 2>/dev/null || pc_fail "xargs -I with five arguments failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "[k] <k> kk (k) k.$nl" ] || pc_fail "-I with five arguments wrote '$v', expected '[k] <k> kk (k) k.'"
printf 'm\n' | xargs -I @@ echo 'x@@y' >out 2>/dev/null || pc_fail "xargs -I @@ failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "xmy$nl" ] || pc_fail "-I @@ wrote '$v', expected 'xmy'"
# only leading blanks are dropped: trailing blanks belong to the argument
printf '  d e \n' | xargs -I '{}' echo '<{}>' >out 2>/dev/null || pc_fail "xargs -I with trailing blanks failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "<d e >$nl" ] || pc_fail "-I with a line 'd e ' wrote '$v', expected '<d e >'"
# a constructed argument of 250 bytes is within the guaranteed limit
big=
i=0
while [ $i -lt 25 ]; do big=${big}0123456789; i=$((i + 1)); done
printf '%s\n' "$big" | xargs -I '{}' echo '{}' >out 2>/dev/null || pc_fail "xargs -I with a 250 byte line failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "$big$nl" ] || pc_fail "-I with a 250 byte line did not pass it on unchanged"
# -x is forced on: a command line that cannot fit in -s is an error and nothing runs
printf 'aaaaaaaaaa\n' | xargs -I '{}' -s 12 echo '{}' >out 2>err
rc=$?
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "-I with a line too long for -s exited with status $rc, expected 1-125"
[ -s out ] && pc_fail "-I ran the utility for a command line that does not fit in -s"
pc_pass
