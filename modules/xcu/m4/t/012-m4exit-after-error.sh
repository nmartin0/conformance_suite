# m4exit after an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.m4.012
# m4exit with no argument or a zero argument, after an error has occurred,
# leaves the exit status non-zero.
. "$PC_CORE/pc.sh"
pc_begin XCU.m4.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >in6 <<'EOM'
include(`nonexistent')m4exit
EOM
cat >in7 <<'EOM'
include(`nonexistent')m4exit(0)
EOM
pc_test
m4 in6 >out 2>err </dev/null
[ "$?" -ne 0 ] || pc_fail "m4exit after an earlier error gave status 0"
m4 in7 >out 2>err </dev/null
[ "$?" -ne 0 ] || pc_fail "m4exit(0) after an earlier error gave status 0"
pc_pass
