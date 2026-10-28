# -x lists name, line number, file and text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ctags.005
# -x writes one line per object to standard output with the name, the line
# number, the file name and the text of that line separated by single
# spaces, and creates no tags file.
. "$PC_CORE/pc.sh"
pc_begin XCU.ctags.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >t.c <<'EOS'
typedef int myint;
#define ADD(a, b) ((a) + (b))
int alpha(int x)
{
	return x;
}
EOS
pc_test
ctags -x t.c </dev/null >out 2>err || pc_fail "ctags -x: nonzero status"
[ -e tags ] && pc_fail "-x created a tags file"
grep -Fx 'alpha 3 t.c int alpha(int x)' out >/dev/null || pc_fail "no -x line for alpha: $(cat out)"
grep -Fx 'myint 1 t.c typedef int myint;' out >/dev/null || pc_fail "no -x line for myint"
grep -Fx 'ADD 2 t.c #define ADD(a, b) ((a) + (b))' out >/dev/null || pc_fail "no -x line for ADD"
pc_pass
