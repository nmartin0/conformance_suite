# dd interprets skip, seek and count values that place data beyond 4 GiB: reading with skip at 4294967296 bytes returns the byte stored there, and writing with seek past 4 GiB gives a file of the expected size
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.5-large-files.002
. "$PC_CORE/pc.sh"
pc_begin XCU.1.5-large-files.002
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf X | dd of=big bs=1 seek=4294967296 2>/dev/null || pc_notinuse "a file larger than 4 GiB cannot be created here"
[ -s big ] || pc_notinuse "a file larger than 4 GiB cannot be created here"

pc_test
r=$(dd if=big bs=1 skip=4294967296 count=1 2>/dev/null)
[ "$r" = X ] || pc_fail "dd bs=1 skip=4294967296 read '$r'"
dd if=big of=o2 bs=1048576 skip=4096 2>/dev/null || pc_fail "dd with bs=1048576 skip=4096 failed"
[ "$(cat o2)" = X ] || pc_fail "dd with bs=1048576 skip=4096 copied '$(cat o2)'"
n=$(dd if=big bs=1048576 skip=4095 count=1 2>/dev/null | wc -c | tr -d ' ')
[ "$n" = 1048576 ] || pc_fail "dd skip=4095 count=1 copied $n bytes"
printf Y | dd of=big2 bs=1048576 seek=4097 2>/dev/null || pc_fail "dd seek=4097 failed"
sz=$(ls -l big2 | awk '{ print $5 }')
[ "$sz" = 4296015873 ] || pc_fail "the file written with seek=4097 has size '$sz'"
r=$(dd if=big2 bs=1 skip=4296015872 count=1 2>/dev/null)
[ "$r" = Y ] || pc_fail "the byte at the seek offset is '$r'"
pc_pass
