# pax -x pax archives and extended records
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.008
# "pax -w -x pax f1" gives an archive of 512-byte blocks whose first block
# has the ustar magic and version, which pax lists and extracts; with -o
# times the archive holds atime and mtime records, and -o comment=hello puts
# a comment record in it.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
fld() { dd if=a.p bs=1 skip=$1 count=$2 2>/dev/null | tr -d '\000'; }
pc_test
pax -w -x pax f1 >a.p 2>err </dev/null || pc_fail "pax -w -x pax: nonzero status"
sz=$(wc -c <a.p); sz=$((sz + 0))
[ $((sz % 512)) -eq 0 ] && [ "$sz" -ge 2048 ] || pc_fail "archive length $sz is not a multiple of 512"
[ "$(fld 257 5)" = ustar ] || pc_fail "the first block has magic '$(fld 257 5)'"
[ "$(fld 263 2)" = 00 ] || pc_fail "the first block has version '$(fld 263 2)'"
pax <a.p >o 2>err || pc_fail "listing: nonzero status"
[ "$(cat o)" = f1 ] || pc_fail "the pax archive lists '$(cat o)'"
rm -f f1
pax -r <a.p >o 2>err || pc_fail "extracting: nonzero status"
[ "$(cat f1)" = 12345 ] || pc_fail "f1 was not restored"
pax -w -x pax -o times f1 >a.t 2>err </dev/null || pc_fail "-o times: nonzero status"
tr -d '\000' <a.t | grep -F 'mtime=' >/dev/null || pc_fail "-o times wrote no mtime record"
tr -d '\000' <a.t | grep -F 'atime=' >/dev/null || pc_fail "-o times wrote no atime record"
pax -w -x pax -o comment=hello f1 >a.c 2>err </dev/null || pc_fail "-o comment: nonzero status"
tr -d '\000' <a.c | grep -F 'comment=hello' >/dev/null || pc_fail "-o comment=hello wrote no comment record"
pc_pass
