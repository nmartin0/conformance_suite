# A standard utility other than those the standard names as declaration utilities is not treated as one: with printf, echo and env, a word such as name=~/x gets no tilde expansion and a word such as name=$var is split into fields
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.4-utility-defaults.021
. "$PC_CORE/pc.sh"
pc_begin XCU.1.4-utility-defaults.021
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
nl='
'
HOME=/pc/home; export HOME
y='a b'
pc_test
got=$(printf '%s\n' pcx=~/q pcy=$y)
[ "$got" = "pcx=~/q${nl}pcy=a${nl}b" ] || pc_fail "printf arguments: got '$got'"
got=$(echo pcx=~/q)
[ "$got" = "pcx=~/q" ] || pc_fail "echo argument: got '$got'"
got=$(env pcx=~/q ${PC_SH:-sh} -c 'printf "%s\n" "$pcx"')
[ "$got" = "~/q" ] || pc_fail "env assignment operand: got '$got'"
got=$(printf '%s\n' pcz=$y | wc -l | tr -d ' ')
[ "$got" = 2 ] || pc_fail "printf: pcz=\$y gave $got fields"
pc_pass
