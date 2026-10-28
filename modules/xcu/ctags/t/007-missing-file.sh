# nonexistent operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ctags.007
# A pathname operand that cannot be read gives a non-zero exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.ctags.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >ok.c <<'EOS'
int fine(void)
{
	return 0;
}
EOS
pc_test
ctags nosuch.c </dev/null >out 2>err && pc_fail "ctags of a missing file exited 0"
ctags -x nosuch.c </dev/null >out 2>err && pc_fail "ctags -x of a missing file exited 0"
pc_pass
