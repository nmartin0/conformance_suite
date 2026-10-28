# pax -t: the access time of a file read is put back
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.028
# "pax -w -t" leaves the access time of the archived file as it was before
# pax read it.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.028
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir d || pc_unresolved "setup: mkdir failed"
for f in f1 d/f2; do
	printf 'some data\n' >$f || pc_unresolved "setup: cannot write $f"
	touch -a -d 2001-09-09T01:46:40Z $f || pc_unresolved "setup: touch -a -d failed"
	ls -lu $f | grep 2001 >/dev/null || pc_unresolved "setup: the access time of $f could not be set"
done
pc_test
pax -w -t f1 d >a 2>err </dev/null || pc_fail "pax -w -t: nonzero status"
n=$(pax <a 2>/dev/null | wc -l | tr -d ' ')
[ "$n" -eq 3 ] || pc_fail "the archive holds $n members, expected 3 (f1, d, d/f2)"
for f in f1 d/f2; do
	ls -lu $f | grep 2001 >/dev/null || pc_fail "after pax -w -t the access time of $f is no longer the old one: $(ls -lu $f)"
done
pc_pass
