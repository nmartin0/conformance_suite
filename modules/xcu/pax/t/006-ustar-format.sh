# pax -x ustar header layout
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.006
# For a 6-byte file of mode 640, "pax -w -x ustar" gives an archive whose
# first block has name f1, mode 640, size 6, typeflag 0, magic "ustar"
# followed by NUL, version 00 and a correct checksum, followed by one data
# block and two zero blocks, and whose length is a multiple of 512.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
chmod 640 f1 || pc_unresolved "setup: chmod failed"
fld() { dd if=a.tar bs=1 skip=$1 count=$2 2>/dev/null | tr -d '\000'; }
pc_test
pax -w -x ustar f1 >a.tar 2>err </dev/null || pc_fail "pax -w -x ustar: nonzero status"
sz=$(wc -c <a.tar); sz=$((sz + 0))
[ $((sz % 512)) -eq 0 ] && [ "$sz" -ge 2048 ] || pc_fail "archive length $sz is not a multiple of 512 holding a header, data and two zero blocks"
[ "$(fld 0 100)" = f1 ] || pc_fail "name field is '$(fld 0 100)'"
[ $((0$(fld 100 8))) -eq $((0640)) ] || pc_fail "mode field is '$(fld 100 8)', expected 640"
[ $((0$(fld 124 12))) -eq 6 ] || pc_fail "size field is '$(fld 124 12)', expected 6"
[ "$(fld 156 1)" = 0 ] || pc_fail "typeflag is '$(fld 156 1)', expected 0"
[ "$(fld 257 5)" = ustar ] || pc_fail "magic is '$(fld 257 5)'"
set -- $(od -An -tx1 -j262 -N1 a.tar)
[ "$1" = 00 ] || pc_fail "the magic is not terminated by NUL"
[ "$(fld 263 2)" = 00 ] || pc_fail "version is '$(fld 263 2)', expected 00"
sum=$(od -An -v -tu1 -N512 a.tar | awk '{ for (i = 1; i <= NF; i++) { n++; if (n >= 149 && n <= 156) s += 32; else s += $i } } END { print s }')
[ $((0$(fld 148 8))) -eq "$sum" ] || pc_fail "checksum field '$(fld 148 8)' does not match the header sum $sum"
z=$(dd if=a.tar bs=512 skip=2 count=2 2>/dev/null | od -An -v -tx1 | tr -d ' 0\n')
[ -z "$z" ] || pc_fail "the two blocks after the data are not zero"
pc_pass
