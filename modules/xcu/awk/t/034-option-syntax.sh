# option syntax
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.034
# -F and -v take their argument attached or separate, repeated -v works, -f
# can be attached, and -- ends the options before the program.
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.034
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'BEGIN { print "from-file" }\n' >prog.awk || pc_unresolved "setup: cannot write the program file"
pc_test
got=$(echo 'a:b' | awk -F: '{ print $2 }' 2>/dev/null)
[ "$got" = "b" ] || pc_fail "-F: : $got"
got=$(echo 'a:b' | awk -F : '{ print $2 }' 2>/dev/null)
[ "$got" = "b" ] || pc_fail "-F : : $got"
got=$(awk -vx=1 'BEGIN { print x }' 2>/dev/null </dev/null)
[ "$got" = "1" ] || pc_fail "-vx=1 : $got"
got=$(awk -v x=1 -v y=2 'BEGIN { print x y }' 2>/dev/null </dev/null)
[ "$got" = "12" ] || pc_fail "-v x=1 -v y=2 : $got"
got=$(awk -fprog.awk 2>/dev/null </dev/null)
[ "$got" = "from-file" ] || pc_fail "-fprog.awk : $got"
got=$(awk -- 'BEGIN { print "ok" }' 2>/dev/null </dev/null)
[ "$got" = "ok" ] || pc_fail "-- before the program : $got"
got=$(echo 'a:b' | awk -F: -f prog.awk -v z=1 2>/dev/null)
[ "$got" = "from-file" ] || pc_fail "options in one command line : $got"
pc_pass
