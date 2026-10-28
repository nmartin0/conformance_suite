# uuencode with a file operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.005
# Encoding the named file gives the same data lines as encoding the same
# bytes from standard input.
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'hello, world\nsecond line\n' >in || pc_unresolved "setup: cannot write input"
pc_test
uuencode dest.bin <in >o1 2>err1
rc=$?
[ "$rc" -eq 0 ] || pc_fail "standard input form: exit status $rc"
uuencode in dest.bin >o2 2>err2
rc=$?
[ "$rc" -eq 0 ] || pc_fail "file operand form: exit status $rc"
l1=$(sed -n 1p o2)
case $l1 in
begin\ *\ dest.bin) ;;
*) pc_fail "begin line has the wrong shape: $l1" ;;
esac
sed '1d' o1 | tr '\140' ' ' >d1
sed '1d' o2 | tr '\140' ' ' >d2
[ -s d1 ] || pc_fail "no encoded lines from standard input"
[ "$(cat d1)" = "$(cat d2)" ] || pc_fail "file operand and standard input give different encodings"
pc_pass
