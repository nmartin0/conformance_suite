# argument list errors
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ex.061
. "$PC_CORE/pc.sh"
pc_begin XCU.ex.061
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'a\nb\n' >f
printf 'a\nb\n' >g
pc_test
printf 'n\nq\n' >s1
ex -s f <s1 >o1 2>e1
[ "$?" -ne 0 ] || pc_fail "n with no further file in the argument list was accepted"
printf 'rew\nq\n' >s2
ex -s <s2 >o2 2>e2
[ "$?" -ne 0 ] || pc_fail "rew with an empty argument list was accepted"
printf 'n\nrew\nw %%.cp\nq!\n' >s3
ex -s f g <s3 >o3 2>e3 || pc_fail "n then rew: exit status $?"
[ -f f.cp ] || pc_fail "rew did not make the first file the current one"
[ ! -f g.cp ] || pc_fail "the second file was still current after rew"
pc_pass
