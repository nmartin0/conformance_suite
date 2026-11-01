# -0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.006
# MUTANT    xargs.zero_ws
# MUTANT    xargs.zero_nl
# MUTANT    xargs.zero_quotes
# MUTANT    xargs.zero_collapse
# MUTANT    xargs.zero_trailing_garbage
# VARIANT   xargs.zero_trailing_used
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.006
LC_ALL=C; export LC_ALL
nl='
'
pc_test
printf 'a b\000c\nd\000 e \000' | xargs -0 printf '[%s]' >out 2>/dev/null || pc_fail "xargs -0 failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "[a b][c${nl}d][ e ]" ] || pc_fail "-0 wrote '$v'"
printf '\047x\047 \042y\042 z\134 w\000' | xargs -0 printf '[%s]' >out 2>/dev/null || pc_fail "xargs -0 with quote characters failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "['x' \"y\" z\\ w]" ] || pc_fail "-0 treated quote or backslash characters specially: '$v'"
printf 'a\000\000b\000' | xargs -0 printf '[%s]' >out 2>/dev/null || pc_fail "xargs -0 with adjacent nulls failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "[a][][b]" ] || pc_fail "adjacent null bytes gave '$v', expected '[a][][b]'"
# input that does not end with a null byte: the trailing bytes may be ignored or used as the last argument
printf 'a\000b' | xargs -0 printf '[%s]' >out 2>/dev/null
v=$(cat out; printf x)
v=${v%x}
case $v in
'[a]' | '[a][b]') ;;
*) pc_fail "input a, null, b gave '$v', expected [a] (b ignored) or [a][b]" ;;
esac
pc_pass
