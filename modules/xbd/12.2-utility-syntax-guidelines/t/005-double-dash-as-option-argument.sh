# A -- that is the option-argument of an option is its value, not the end of the options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.005
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.005
LC_ALL=POSIX
export LC_ALL
printf 'a--b\nplain\n' >g6 || pc_unresolved "cannot create g6"
pc_test
got=$(grep -e -- g6 2>/dev/null </dev/null)
[ "$got" = "a--b" ] || pc_fail "grep -e -- g6 gave '$got', expected the line a--b"
set -- -a -- -b
OPTIND=1
getopts a:b o
r1=$?
v1=$o
a1=$OPTARG
getopts a:b o
r2=$?
v2=$o
getopts a:b o
r3=$?
[ "$r1" -eq 0 ] && [ "$v1" = a ] && [ "$a1" = "--" ] || pc_fail "getopts a:b on -a -- -b: the first call gave status $r1, name '$v1', OPTARG '$a1'; expected a with the value --"
[ "$r2" -eq 0 ] && [ "$v2" = b ] || pc_fail "getopts a:b on -a -- -b: the second call gave status $r2, name '$v2'; expected the option b"
[ "$r3" -eq 1 ] || pc_fail "getopts a:b on -a -- -b: the third call gave status $r3, expected 1 (no more options)"
pc_pass
