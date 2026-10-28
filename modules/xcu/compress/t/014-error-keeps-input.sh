# a failed decompression leaves the input alone
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.014
# "compress -d g.Z" on a file that is not compressed data either succeeds or
# fails, and if it fails (nonzero status) g.Z is unchanged and still
# present.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'this is not compressed data\n' >g.Z || pc_unresolved "setup: cannot write input"
cat g.Z >g.keep || pc_unresolved "setup: cannot copy input"
pc_test
compress -d g.Z </dev/null >out 2>err
rc=$?
if [ "$rc" -eq 0 ]; then
	pc_unresolved "the data was accepted as compressed data"
fi
[ -f g.Z ] || pc_fail "the input file is gone after an error"
cmp -s g.Z g.keep || pc_fail "the input file was changed after an error"
pc_pass
