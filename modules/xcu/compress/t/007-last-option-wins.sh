# compress -g and -m algo, last one decides
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.007
# With "-g -m lzw" the output suffix is .Z; with "-m lzw -g" it is .gz; with
# two -m options the last one decides; none of these is an error.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.007
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
pc_test
try() {
	want=$1
	shift
	rm -f f f.Z f.gz
	cat orig >f
	compress "$@" f </dev/null >out 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "compress $*: exit status $rc"
	[ -f "f.$want" ] || pc_fail "compress $*: f.$want was not created"
}
try Z -g -m lzw
try gz -m lzw -g
try Z -m gzip -m lzw
try gz -m lzw -m gzip
pc_pass
