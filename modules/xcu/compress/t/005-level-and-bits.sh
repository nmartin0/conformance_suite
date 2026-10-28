# compress -b values
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.005
# "-b 12" with lzw, and "-b 1" and "-b 9" with -g, are accepted and the
# compressed file decompresses to the original bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.005
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
pc_test
for opt in "-b 12" "-g -b 1" "-g -b 9"; do
	rm -f f f.Z f.gz
	cat orig >f
	compress $opt f </dev/null >out 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "compress $opt: exit status $rc"
	[ -e f ] && pc_fail "compress $opt: input was not replaced"
	compress -d f.* </dev/null >out 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "decompressing after $opt: exit status $rc"
	cmp -s f orig || pc_fail "round trip after $opt differs"
done
pc_pass
