# zcat of data that is not compressed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.zcat.004
# "zcat" on a file that is not compressed data may succeed (UNRESOLVED);
# if it exits nonzero the file is unchanged.
. "$PC_CORE/pc.sh"
pc_begin XCU.zcat.004
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'this is not compressed data\n' >g.Z || pc_unresolved "setup: cannot write input"
cat g.Z >g.keep || pc_unresolved "setup: cannot copy input"
pc_test
zcat g.Z </dev/null >o 2>err
rc=$?
if [ "$rc" -eq 0 ]; then
	pc_unresolved "the data was accepted as compressed data"
fi
[ -f g.Z ] || pc_fail "the input file is gone after an error"
cmp -s g.Z g.keep || pc_fail "the input file was changed after an error"
pc_pass
