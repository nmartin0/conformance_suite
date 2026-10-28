# With -l, scale starts at 20 and s, c, a, l, e and j are defined, returning results at the scale in force and leaving scale unchanged.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.007
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.007
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
b() { printf '%b\n' "$1" | bc -l 2>/dev/null; }
z() { r=$1; case $r in 0.*) r=${r#0} ;; -0.*) r=-${r#-0} ;; esac; printf '%s' "$r"; }
[ "$(b 'scale')" = "20" ] || pc_fail "scale was not 20 with -l"
[ "$(b 'scale=3\nc(0)')" = "1.000" ] || pc_fail "c(0) at scale 3 not 1.000"
[ "$(b 'scale=3\ne(0)')" = "1.000" ] || pc_fail "e(0) at scale 3 not 1.000"
[ "$(b 'scale=3\nl(1)')" = "0" ] || pc_fail "l(1) not 0"
[ "$(b 'scale=3\ns(0)')" = "0" ] || pc_fail "s(0) not 0"
[ "$(b 'scale=3\na(0)')" = "0" ] || pc_fail "a(0) not 0"
got=$(b 'scale=12\ne(1)')
case $got in 2.71828182*) ;; *) pc_fail "e(1) was not about 2.71828182" ;; esac
got=$(b 'scale=12\nl(10)')
case $got in 2.3025850*) ;; *) pc_fail "l(10) was not about 2.302585" ;; esac
got=$(z "$(b 'scale=12\ns(1)')")
case $got in .8414709*) ;; *) pc_fail "s(1) was not about .8414709" ;; esac
got=$(z "$(b 'scale=12\nc(1)')")
case $got in .5403023*) ;; *) pc_fail "c(1) was not about .5403023" ;; esac
got=$(z "$(b 'scale=12\na(1)')")
case $got in .7853981*) ;; *) pc_fail "a(1) was not about .7853981 (pi/4)" ;; esac
got=$(b 'scale=12\n4*a(1)')
case $got in 3.14159265*) ;; *) pc_fail "4*a(1) was not about 3.14159265" ;; esac
[ "$(b 'scale=7\nx=e(1)\nscale')" = "7" ] || pc_fail "scale was changed by a library function"
[ "$(b 'scale=3\nj(0,0)')" = "1.000" ] || pc_fail "j(0,0) at scale 3 not 1.000"
[ "$(b 'scale=3\nj(1,0)')" = "0" ] || pc_fail "j(1,0) not 0"
got=$(b '1/3')
case $got in .3333333333333333333*|0.3333333333333333333*) ;; *) pc_fail "1/3 at the initial scale of 20 wrong" ;; esac
pc_pass
