# -D, -U and -I as for c17, in order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cflow.006
# -D defines a macro, -U removes it, the later of -D and -U for a name wins,
# and -I adds a directory searched for quoted include files.
. "$PC_CORE/pc.sh"
pc_begin XCU.cflow.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >pp.c <<'EOS'
int
extra(void)
{
	return 0;
}
int
user(void)
{
#ifdef WITHCALL
	return extra();
#else
	return 1;
#endif
}
EOS
mkdir inc || pc_unresolved "setup: cannot make a directory"
cat >inc/call.h <<'EOS'
#define CALLEXTRA() extra()
EOS
cat >ph.c <<'EOS'
#include "call.h"
int
extra(void)
{
	return 0;
}
int
user(void)
{
	return CALLEXTRA();
}
EOS
col() { awk -v n="$2" 'index($0, " " n ":") > 0 { print index($0, " " n ":"); exit }' "$1"; }
pc_test
cflow pp.c </dev/null >o0 2>err || pc_fail "cflow: nonzero status"
cu=$(col o0 user); ce=$(col o0 extra)
[ -n "$cu" ] && [ -n "$ce" ] || pc_fail "user or extra missing: $(cat o0)"
[ "$ce" -gt "$cu" ] && pc_fail "extra is under user without -D"
cflow -D WITHCALL pp.c </dev/null >o1 2>err || pc_fail "cflow -D: nonzero status"
cu=$(col o1 user); ce=$(col o1 extra)
[ -n "$cu" ] && [ -n "$ce" ] || pc_fail "user or extra missing with -D: $(cat o1)"
[ "$ce" -gt "$cu" ] || pc_fail "-D WITHCALL did not make extra a callee of user: $(cat o1)"
cflow -D WITHCALL -U WITHCALL pp.c </dev/null >o2 2>err || pc_fail "cflow -D -U: nonzero status"
cu=$(col o2 user); ce=$(col o2 extra)
[ -n "$cu" ] && [ -n "$ce" ] || pc_fail "user or extra missing with -D -U: $(cat o2)"
[ "$ce" -gt "$cu" ] && pc_fail "-U after -D did not remove the macro"
cflow -U WITHCALL -D WITHCALL pp.c </dev/null >o3 2>err || pc_fail "cflow -U -D: nonzero status"
cu=$(col o3 user); ce=$(col o3 extra)
[ -n "$cu" ] && [ -n "$ce" ] || pc_fail "user or extra missing with -U -D: $(cat o3)"
[ "$ce" -gt "$cu" ] || pc_fail "-D after -U did not define the macro: $(cat o3)"
cflow -I inc ph.c </dev/null >o4 2>err || pc_fail "cflow -I: nonzero status"
cu=$(col o4 user); ce=$(col o4 extra)
[ -n "$cu" ] && [ -n "$ce" ] || pc_fail "user or extra missing with -I: $(cat o4)"
[ "$ce" -gt "$cu" ] || pc_fail "-I inc did not find call.h: $(cat o4)"
pc_pass
