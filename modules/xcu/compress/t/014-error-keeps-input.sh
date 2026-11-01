# a failed decompression leaves the input alone
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.014
# VARIANT   compress.d_garbage_accepted
# VARIANT   compress.lzw_other_format
# MUTANT    compress.err_removes_input
# MUTANT    compress.err_modifies_input
# MUTANT    compress.err_truncates_input
# MUTANT    compress.err_c_removes_input
# MUTANT    compress.err_gz_removes_input
# When compress fails on an input file (nonzero status), the input file is
# still present and unchanged: "compress -d" of a file that is not
# compressed data or of a truncated gzip file (either may succeed), and
# "compress -f f" when the output name f.Z is a directory and cannot be
# replaced by a file (an error on every system; if it succeeded no output
# file could exist, so it is not judged).
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'this is not compressed data\n' >g.Z || pc_unresolved "setup: cannot write input"
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
compress -g -c orig </dev/null >full.gz 2>/dev/null || pc_unresolved "setup: compress -g -c failed"
size=$(wc -c <full.gz)
dd if=full.gz of=h.gz bs=1 count=$((size / 2)) 2>/dev/null || pc_unresolved "setup: cannot truncate"
cat orig >f || pc_unresolved "setup: cannot copy input"
mkdir f.Z || pc_unresolved "setup: cannot make a directory"
for x in g.Z h.gz f; do cat "$x" >"$x.keep" || pc_unresolved "setup: cannot copy input"; done
pc_test
failed=0
# check NAME: after an error status the input is present and unchanged
check() {
	[ -f "$1" ] || pc_fail "$1 is gone after an error"
	cmp -s "$1" "$1.keep" || pc_fail "$1 was changed after an error"
}
compress -d g.Z </dev/null >out 2>err
rc=$?
if [ "$rc" -ne 0 ]; then failed=1; check g.Z; fi
compress -d h.gz </dev/null >out 2>err
rc=$?
if [ "$rc" -ne 0 ]; then failed=1; check h.gz; fi
compress -f f </dev/null >out 2>err
rc=$?
if [ "$rc" -ne 0 ]; then failed=1; check f; fi
if [ "$failed" -eq 0 ]; then
	pc_unresolved "no input produced an error"
fi
pc_pass
