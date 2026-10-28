# uuencode of non-text bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.009
# Input bytes 0x00 0xff 0x80 encode to "AP+A" with -m and, without -m, to
# the data line "#" then a space-or-grave, "/", "^", a space-or-grave.
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '\000\377\200' >in || pc_unresolved "setup: cannot write input"
sz=$(wc -c <in); sz=$((sz + 0))
[ "$sz" -eq 3 ] || pc_unresolved "setup: input is $sz bytes"
pc_test
uuencode -m dest.bin <in >o1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "-m: exit status $rc"
l=$(sed -n 2p o1)
[ "$l" = 'AP+A' ] || pc_fail "-m data line is '$l', expected 'AP+A'"
uuencode dest.bin <in >o2 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
l=$(sed -n 2p o2 | tr '\140' ' ')
[ "$l" = '# /^ ' ] || pc_fail "data line differs from the expected encoding"
pc_pass
