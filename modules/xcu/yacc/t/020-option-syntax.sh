# option syntax
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.yacc.020
# -b and -p take their value attached or separate, options can be grouped,
# and -- ends the options.
. "$PC_CORE/pc.sh"
pc_begin XCU.yacc.020
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_unresolved "setup: c17 is not available"
cat >g.y <<'EOY'
%%
s : 'a'
  ;
EOY
pc_test
yacc -dv -b one -- g.y </dev/null >y.out 2>y.err || pc_fail "yacc -dv -b one -- g.y failed"
for f in one.tab.c one.tab.h one.output; do
	[ -f "$f" ] || pc_fail "$f was not written by yacc -dv -b one"
done
yacc -bq -d g.y </dev/null >y.out 2>y.err || pc_fail "yacc -bq failed"
for f in q.tab.c q.tab.h; do
	[ -f "$f" ] || pc_fail "$f was not written by yacc -bq"
done
yacc -p xx -b r g.y </dev/null >y.out 2>y.err || pc_fail "yacc -p xx -b r failed"
grep xxparse r.tab.c >/dev/null 2>&1 || pc_fail "-p xx (separate) did not rename yyparse in r.tab.c"
yacc -pzz -b s g.y </dev/null >y.out 2>y.err || pc_fail "yacc -pzz -b s failed"
grep zzparse s.tab.c >/dev/null 2>&1 || pc_fail "-pzz (attached) did not rename yyparse in s.tab.c"
pc_pass
