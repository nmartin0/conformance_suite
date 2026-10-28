# cmp -l reports byte numbers above 4 GiB: two files of 4294967297 bytes that differ only in the last byte give byte number 4294967297 with the two values
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.1.5-large-files.003
. "$PC_CORE/pc.sh"
pc_begin XCU.1.5-large-files.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf X | dd of=big bs=1 seek=4294967296 2>/dev/null || pc_notinuse "a file larger than 4 GiB cannot be created here"
[ -s big ] || pc_notinuse "a file larger than 4 GiB cannot be created here"

printf Y | dd of=big2 bs=1 seek=4294967296 2>/dev/null || pc_notinuse "a second large file cannot be created"
pc_test
r=$(cmp -l big big2 2>cmp.err) 
rc=$?
[ "$rc" = 1 ] || pc_fail "cmp exit status $rc: $(cat cmp.err)"
r=$(echo $r)
[ "$r" = "4294967297 130 131" ] || pc_fail "cmp -l printed '$r'"
pc_pass
