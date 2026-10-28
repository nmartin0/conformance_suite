# pax -b: the output is blocked, and blocking is found on input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.021
# With -b 4096 (and with -b 32256) each of the formats ustar, cpio and pax
# writes an archive whose length is a multiple of the block size, and pax
# lists and extracts it without -b.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.021
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
pc_test
for fmt in ustar cpio pax; do
	for bs in 4096 32256; do
		pax -w -b "$bs" -x "$fmt" f1 >a.$fmt.$bs 2>err </dev/null || pc_fail "pax -w -b $bs -x $fmt: nonzero status"
		sz=$(wc -c <a.$fmt.$bs); sz=$((sz + 0))
		[ "$sz" -gt 0 ] && [ $((sz % bs)) -eq 0 ] || pc_fail "-x $fmt -b $bs: the archive is $sz octets long, not a multiple of the block size"
		got=$(pax <a.$fmt.$bs 2>err)
		[ "$got" = f1 ] || pc_fail "-x $fmt -b $bs: the blocked archive lists '$got' without -b"
		rm -f f1
		pax -r <a.$fmt.$bs >o 2>err || pc_fail "-x $fmt -b $bs: extraction without -b failed"
		[ "$(cat f1 2>/dev/null)" = 12345 ] || pc_fail "-x $fmt -b $bs: f1 was not restored from the blocked archive"
	done
done
pc_pass
