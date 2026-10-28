# dd skip= on a pipe
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.dd.009
. "$PC_CORE/pc.sh"
pc_begin XCU.dd.009
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# On a non-seekable input the skipped blocks are read and discarded. Blocks
# of one byte, so that short pipe reads cannot matter.
gen() { g=$1; while [ "$g" -lt "$2" ]; do printf 'L%02d-%095d\n' "$g" "$g"; g=$((g+1)); done; }
gen 2 5 >exp25 || pc_fail "cannot write exp25"
pc_test
gen 0 5 | dd ibs=1 skip=200 of=out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "dd skip=200 ibs=1 on a pipe exited with status $rc"
cmp -s exp25 out || pc_fail "skipping 200 bytes of a pipe did not leave bytes 200 to 499"
pc_pass
