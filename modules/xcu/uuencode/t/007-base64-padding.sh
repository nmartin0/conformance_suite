# uuencode -m padding
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.007
# With -m, one input byte "a" gives "YQ==" and two input bytes "ab" gives
# "YWI=" as the single data line.
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
:
pc_test
printf 'a' | uuencode -m dest.bin >o1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "one byte: exit status $rc"
l=$(sed -n 2p o1)
[ "$l" = 'YQ==' ] || pc_fail "one byte encodes to '$l', expected 'YQ=='"
printf 'ab' | uuencode -m dest.bin >o2 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "two bytes: exit status $rc"
l=$(sed -n 2p o2)
[ "$l" = 'YWI=' ] || pc_fail "two bytes encode to '$l', expected 'YWI='"
pc_pass
