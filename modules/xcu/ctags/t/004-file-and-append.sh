# -f names the tags file and -a appends
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ctags.004
# -f writes the locators into the named file instead of tags, and -a adds to
# the existing file keeping its earlier lines.
. "$PC_CORE/pc.sh"
pc_begin XCU.ctags.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >a.c <<'EOS'
int first(void)
{
	return 1;
}
EOS
cat >b.c <<'EOS'
int second(void)
{
	return 2;
}
EOS
TAB=$(printf '\t')
pc_test
ctags -f mytags a.c </dev/null >out 2>err || pc_fail "ctags -f: nonzero status"
[ -f mytags ] || pc_fail "-f did not write the named file"
[ -e tags ] && pc_fail "-f also wrote a file named tags"
grep "^first$TAB" mytags >/dev/null || pc_fail "no tag for first in the named file"
grep "^first$TAB" mytags >keep
ctags -a -f mytags b.c </dev/null >out 2>err || pc_fail "ctags -a: nonzero status"
grep "^second$TAB" mytags >/dev/null || pc_fail "-a did not add the new tag"
grep "^first$TAB" mytags >now
cmp keep now >/dev/null 2>&1 || pc_fail "-a changed the earlier tag line"
[ -e tags ] && pc_fail "-a with -f wrote a file named tags"
pc_pass
