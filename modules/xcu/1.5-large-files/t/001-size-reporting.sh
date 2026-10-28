# For a file larger than 4 GiB (a sparse file of 4294967297 bytes), ls -l prints the exact size, test -s succeeds, find -size matches the exact block and byte counts, and du and df succeed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.5-large-files.001
. "$PC_CORE/pc.sh"
pc_begin XCU.1.5-large-files.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf X | dd of=big bs=1 seek=4294967296 2>/dev/null || pc_notinuse "a file larger than 4 GiB cannot be created here"
[ -s big ] || pc_notinuse "a file larger than 4 GiB cannot be created here"

pc_test
sz=$(ls -l big | awk '{ print $5 }')
[ "$sz" = 4294967297 ] || pc_fail "ls -l printed size '$sz'"
[ -s big ] || pc_fail "test -s failed"
[ -f big ] || pc_fail "test -f failed"
[ "$(find big -size 4294967297c -print)" = big ] || pc_fail "find -size with the exact byte count did not match"
[ "$(find big -size 8388609 -print)" = big ] || pc_fail "find -size with the exact block count did not match"
[ "$(find big -size +8388608 -print)" = big ] || pc_fail "find -size +8388608 did not match"
[ -z "$(find big -size -8388609 -print)" ] || pc_fail "find -size -8388609 matched"
du -k big >du.out 2>du.err || pc_fail "du failed: $(cat du.err)"
case $(cat du.out) in [0-9]*) ;; *) pc_fail "du printed '$(cat du.out)'" ;; esac
df . >df.out 2>df.err || pc_fail "df failed: $(cat df.err)"
pc_pass
