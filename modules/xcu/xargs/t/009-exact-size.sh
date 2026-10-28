# -x with -n and -s.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.009
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
printf 'aaaa bbbb\n' | xargs -x -n 2 -s 12 echo >out 2>/dev/null
rc=$?
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "with -x the status was $rc, expected 1-125"
v=$(cat out; printf x)
v=${v%x}
case $v in
*aaaa*bbbb*) pc_fail "with -x a command line that did not fit was run: '$v'" ;;
esac
pc_pass
