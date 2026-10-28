# pax -x cpio header layout
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.007
# "pax -w -x cpio f1" gives an archive starting with the magic 070707, with
# c_mode 0100640 in bytes 18 to 23, c_namesize 3 and c_filesize 6 (octal),
# the name f1 after the 76-byte header, and a trailer entry named
# TRAILER!!!.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
chmod 640 f1 || pc_unresolved "setup: chmod failed"
fld() { dd if=a.cpio bs=1 skip=$1 count=$2 2>/dev/null | tr -d '\000'; }
pc_test
pax -w -x cpio f1 >a.cpio 2>err </dev/null || pc_fail "pax -w -x cpio: nonzero status"
[ "$(fld 0 6)" = 070707 ] || pc_fail "c_magic is '$(fld 0 6)'"
[ "$(fld 18 6)" = 100640 ] || pc_fail "c_mode is '$(fld 18 6)', expected 100640"
[ $((0$(fld 59 6))) -eq 3 ] || pc_fail "c_namesize is '$(fld 59 6)', expected 3"
[ $((0$(fld 65 11))) -eq 6 ] || pc_fail "c_filesize is '$(fld 65 11)', expected 6"
[ "$(fld 76 2)" = f1 ] || pc_fail "the name after the header is '$(fld 76 2)'"
tr -d '\000' <a.cpio | grep -F 'TRAILER!!!' >/dev/null || pc_fail "no TRAILER!!! entry"
pax <a.cpio >o 2>err || pc_fail "listing the cpio archive: nonzero status"
[ "$(cat o)" = f1 ] || pc_fail "the cpio archive lists '$(cat o)'"
pc_pass
