# dd conv=notrunc
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.012
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.012
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# conv=notrunc keeps what dd does not write, with and without seek=.
rep() { r=0; while [ "$r" -lt "$1" ]; do printf '%s' "$2"; r=$((r+1)); done; }
rep 50 X >new || pc_fail "cannot write new"
{ rep 50 X; rep 150 o; } >exp1 || pc_fail "cannot write exp1"
{ rep 100 o; rep 50 X; rep 150 o; } >exp2 || pc_fail "cannot write exp2"
pc_test
rep 200 o >out || pc_fail "cannot write out"
dd if=new of=out conv=notrunc </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd conv=notrunc exited with status $rc"
cmp -s exp1 out || pc_fail "conv=notrunc: the tail of the old file was not kept"
rep 300 o >out || pc_fail "cannot write out"
dd if=new of=out obs=100 seek=1 conv=notrunc </dev/null 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd seek=1 conv=notrunc exited with status $rc"
cmp -s exp2 out || pc_fail "seek=1 conv=notrunc: the blocks around the written data were not kept"
pc_pass
