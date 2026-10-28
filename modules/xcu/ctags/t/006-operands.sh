# .c and .h operands together
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ctags.006
# Files whose names end in .c or .h are scanned as C source, and several
# operands give tags from each file with its own name in the file column.
. "$PC_CORE/pc.sh"
pc_begin XCU.ctags.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >a.c <<'EOS'
int from_c(void)
{
	return 1;
}
EOS
cat >b.h <<'EOS'
typedef long from_h;
EOS
TAB=$(printf '\t')
pc_test
ctags a.c b.h </dev/null >out 2>err || pc_fail "ctags: nonzero status"
l1=$(grep "^from_c$TAB" tags | sed -n 1p)
l2=$(grep "^from_h$TAB" tags | sed -n 1p)
[ -n "$l1" ] || pc_fail "no tag for the function in a.c"
[ -n "$l2" ] || pc_fail "no tag for the typedef in b.h"
case "$l1" in *"${TAB}a.c${TAB}"*) ;; *) pc_fail "from_c is not located in a.c: $l1" ;; esac
case "$l2" in *"${TAB}b.h${TAB}"*) ;; *) pc_fail "from_h is not located in b.h: $l2" ;; esac
pc_pass
