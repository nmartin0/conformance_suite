# address separators and defaults
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.019
# Omitted sides of a comma or semicolon pair take the defaults, a semicolon
# makes the first address current before the second is evaluated, extra
# addresses are evaluated and dropped, and an address on a command that takes
# none is an error.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'XX'
,p
2p
;p
3,p
,2p
4p
4;p
1p
;3p
,,p
4p
2;/o/p
1p
2,/o/p
1,2,4p
XX
printf 'one two three four five two two three four five three one two four four one one two three five four two three four one two two three four' | tr ' ' '\n' >exp
printf '1q\n' >e1
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "separators gave '$(tr '\n' ' ' <o)', expected '$(tr '\n' ' ' <exp)'"
ed -s f <e1 >o1 2>err
[ "$(sed -n 1p o1)" = '?' ] || pc_fail "an address on q gave '$(cat o1)', expected ?"
pc_pass
