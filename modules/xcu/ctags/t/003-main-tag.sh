# main is tagged from the file name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ctags.003
# The tag for main in a file is the file name with leading directories and
# the trailing .c removed, prefixed by M.
. "$PC_CORE/pc.sh"
pc_begin XCU.ctags.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir sub || pc_unresolved "setup: cannot make a directory"
cat >sub/prog.c <<'EOS'
int helper(void)
{
	return 1;
}
int main(void)
{
	return helper();
}
EOS
TAB=$(printf '\t')
pc_test
ctags sub/prog.c </dev/null >out 2>err || pc_fail "ctags: nonzero status"
[ -f tags ] || pc_fail "no tags file"
grep "^Mprog$TAB" tags >/dev/null || pc_fail "no tag Mprog for main in sub/prog.c"
grep "^helper$TAB" tags >/dev/null || pc_fail "no tag for helper"
pc_pass
