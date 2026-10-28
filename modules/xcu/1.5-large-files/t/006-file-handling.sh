# touch, chmod, chown, chgrp, ln, mv and rm operate on a file larger than 4 GiB without changing its size: the size is still 4294967297 bytes after each, the link count follows ln and rm
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.5-large-files.006
. "$PC_CORE/pc.sh"
pc_begin XCU.1.5-large-files.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf X | dd of=big bs=1 seek=4294967296 2>/dev/null || pc_notinuse "a file larger than 4 GiB cannot be created here"
[ -s big ] || pc_notinuse "a file larger than 4 GiB cannot be created here"

uid=$(id -u); gid=$(id -g)
pc_test
size() { ls -l "$1" | awk '{ print $5 }'; }
touch big || pc_fail "touch failed"
touch -t 200101010000 big || pc_fail "touch -t failed"
chmod 600 big || pc_fail "chmod failed"
chown "$uid" big || pc_fail "chown failed"
chgrp "$gid" big || pc_fail "chgrp failed"
[ "$(size big)" = 4294967297 ] || pc_fail "the size changed after touch, chmod, chown and chgrp"
ln big big.l || pc_fail "ln failed"
set -- $(ls -l big)
[ "$2" = 2 ] || pc_fail "the link count is $2 after ln"
mv big big.m || pc_fail "mv failed"
[ ! -e big ] || pc_fail "mv left the old name"
[ "$(size big.m)" = 4294967297 ] || pc_fail "the size changed after mv"
rm big.l || pc_fail "rm failed"
set -- $(ls -l big.m)
[ "$2" = 1 ] || pc_fail "the link count is $2 after rm"
rm big.m || pc_fail "rm failed for the last link"
[ ! -e big.m ] || pc_fail "the file still exists"
pc_pass
