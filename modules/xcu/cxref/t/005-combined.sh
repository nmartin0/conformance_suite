# -c merges the files into one sorted listing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cxref.005
# Without -c each input file has its own portion starting with its file
# name; with -c the files are cross-referenced together in one sorted list
# of symbols.
. "$PC_CORE/pc.sh"
pc_begin XCU.cxref.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >a.c <<'EOS'
int zeta;
int
zfun(void)
{
	return zeta;
}
EOS
cat >b.c <<'EOS'
extern int zeta;
int
afun(void)
{
	return zeta;
}
EOS
heads() { awk '/^[^ \t]/ && ($1 == "zeta" || $1 == "zfun" || $1 == "afun") { print $1 }' "$1"; }
pc_test
cxref a.c b.c </dev/null >sep 2>err || pc_fail "cxref: nonzero status"
la=$(grep -n '^[^ 	]*a\.c' sep | sed -n 1p | cut -d: -f1)
lb=$(grep -n '^[^ 	]*b\.c' sep | sed -n 1p | cut -d: -f1)
[ -n "$la" ] && [ -n "$lb" ] || pc_fail "no separate file name lines: $(cat sep)"
[ "$la" -lt "$lb" ] || pc_fail "portions are not in operand order"
cxref -c a.c b.c </dev/null >comb 2>err || pc_fail "cxref -c: nonzero status"
heads comb >hc
for s in zeta zfun afun; do
	grep -w "$s" comb >/dev/null || pc_fail "-c listing has no entry for $s: $(cat comb)"
	grep -x "$s" hc >/dev/null || pc_unresolved "cannot find the entry for $s: the layout of the listing is unspecified"
done
sort -c hc 2>/dev/null || pc_fail "the combined listing is not one sorted list: $(tr '\n' ' ' <hc)"
pc_pass
