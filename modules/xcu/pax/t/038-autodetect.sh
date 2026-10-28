# pax: the archive format is recognised on input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.038
# Archives written with -x ustar, -x cpio and -x pax are recognised without
# any option on input: pax lists and extracts all three the same way.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.038
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir d || pc_unresolved "setup: mkdir failed"
printf 'one\n' >f1 && printf 'two\n' >d/f2 || pc_unresolved "setup: cannot write files"
pc_test
for fmt in ustar cpio pax; do
	pax -w -x $fmt f1 d >a.$fmt 2>err </dev/null || pc_fail "pax -w -x $fmt: nonzero status"
	got=$(pax <a.$fmt 2>err | sort | tr '\n' ' ')
	[ "$got" = 'd d/f2 f1 ' ] || pc_fail "the $fmt archive lists '$got' without -x, expected d, d/f2 and f1"
	got=$(pax -f a.$fmt 2>err | sort | tr '\n' ' ')
	[ "$got" = 'd d/f2 f1 ' ] || pc_fail "the $fmt archive named by -f lists '$got'"
	rm -rf f1 d
	pax -r <a.$fmt >o 2>err || pc_fail "extracting the $fmt archive: nonzero status"
	[ "$(cat f1 2>/dev/null)" = one ] || pc_fail "f1 was not restored from the $fmt archive"
	[ "$(cat d/f2 2>/dev/null)" = two ] || pc_fail "d/f2 was not restored from the $fmt archive"
done
pc_pass
