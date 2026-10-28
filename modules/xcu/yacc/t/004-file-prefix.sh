# -b file prefix
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.004
# -b pre names the outputs pre.tab.c, pre.tab.h and pre.output and no y.*
# files are written.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%%
s : 'a' ;
EOY
pc_test
yacc -d -v -b pre g.y </dev/null >y.out 2>y.err || pc_fail "yacc -b: nonzero status"
for f in pre.tab.c pre.tab.h pre.output; do
	[ -s "$f" ] || pc_fail "$f was not written"
done
for f in y.tab.c y.tab.h y.output; do
	[ -e "$f" ] && pc_fail "$f was written although -b was given"
done
pc_pass
