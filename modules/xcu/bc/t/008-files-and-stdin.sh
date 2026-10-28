# bc runs each file operand in order and then reads the standard input, so definitions and values from files are available to later input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.008
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'define f(x) {\nreturn(x+10)\n}\n' >fdef || pc_fail "cannot create input"
printf 'a=3\n' >fvar || pc_fail "cannot create input"
printf '1+1\n' >fexpr || pc_fail "cannot create input"
pc_test
got=$(printf 'f(1)\n' | bc fdef 2>/dev/null)
[ "$got" = "11" ] || pc_fail "a function defined in a file was not available to standard input"
got=$(printf 'a+1\n' | bc fdef fvar 2>/dev/null)
[ "$got" = "4" ] || pc_fail "two files and standard input wrong"
got=$(printf '5\n' | bc fexpr 2>/dev/null)
[ "$got" = "2${NL}5" ] || pc_fail "output of a file was not followed by output of standard input"
got=$(printf '5\n' | bc fexpr fexpr 2>/dev/null)
[ "$got" = "2${NL}2${NL}5" ] || pc_fail "two file operands were not both executed"
got=$(printf '7\n' | bc 2>/dev/null)
[ "$got" = "7" ] || pc_fail "standard input alone wrong"
pc_pass
