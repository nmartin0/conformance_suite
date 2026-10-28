# ? for errors, h and H
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.017
# An invalid command writes "?" and ed stays in command mode with the
# current line unchanged; h then writes an explanation; with H on, the
# explanation follows each "?"; the exit status after a command error is
# nonzero.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf 'xyz\n=\nQ\n' >s1
printf '10p\n=\nQ\n' >s2
printf 'xyz\nh\nQ\n' >s3
printf 'H\nxyz\nQ\n' >s4
printf '1s/nomatch/x/\nQ\n' >s5
printf '?\n5\n' >exp
pc_test
ed -s f <s1 >o1 2>err
rc=$?
cmp -s o1 exp || pc_fail "an invalid command gave '$(cat o1)', expected ? then the line count 5"
[ "$rc" -ne 0 ] || pc_fail "exit status 0 after a command error"
ed -s f <s2 >o2 2>err
[ "$(sed -n 1p o2)" = '?' ] || pc_fail "an out-of-range address gave '$(cat o2)', expected ? first"
ed -s f <s3 >o3 2>err
[ "$(sed -n 1p o3)" = '?' ] || pc_fail "no ? for the invalid command"
[ -n "$(sed -n 2p o3)" ] || pc_fail "h wrote no explanation"
ed -s f <s4 >o4 2>err
[ "$(sed -n 1p o4)" = '?' ] || pc_fail "H mode: no ? for the invalid command"
[ -n "$(sed -n 2p o4)" ] || pc_fail "H mode wrote no explanation after the ?"
ed -s f <s5 >o5 2>err
[ "$(sed -n 1p o5)" = '?' ] || pc_fail "a substitution that fails everywhere gave no ?"
pc_pass
