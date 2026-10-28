# tags file sorted by identifier
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ctags.002
# The identifiers in the tags file are in ascending order of the collation
# sequence of the POSIX locale.
. "$PC_CORE/pc.sh"
pc_begin XCU.ctags.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >t.c <<'EOS'
int zed(void)
{
	return 0;
}
int Alpha(void)
{
	return 1;
}
int beta(void)
{
	return 2;
}
int alpha(void)
{
	return 3;
}
int _under(void)
{
	return 4;
}
EOS
pc_test
ctags t.c </dev/null >out 2>err || pc_fail "ctags: nonzero status"
[ -f tags ] || pc_fail "no tags file"
cut -f1 tags >ids
[ "$(wc -l <ids | tr -d ' ')" -ge 5 ] || pc_fail "fewer than five tags written"
sort -c ids 2>/dev/null || pc_fail "identifiers are not sorted in the POSIX collation order"
for n in zed Alpha beta alpha _under; do
	grep -x "$n" ids >/dev/null || pc_fail "no tag for $n"
done
pc_pass
