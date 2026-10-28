# compress -g and -m deflate give GZIP data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.compress.003
# With -g, with -m gzip and with -m deflate the output file is f.gz and
# starts with the gzip signature bytes 1f 8b and method byte 08; "compress
# -d f.gz" restores the original bytes.
. "$PC_CORE/pc.sh"
pc_begin XCU.compress.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=0; while [ "$i" -lt 200 ]; do printf 'line %d of some repetitive test text\n' "$i"; i=$((i + 1)); done >orig || pc_unresolved "setup: cannot write input"
pc_test
for opt in "-g" "-m gzip" "-m deflate"; do
	rm -f f f.gz
	cat orig >f
	compress $opt f </dev/null >out 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "compress $opt: exit status $rc"
	[ -f f.gz ] || pc_fail "compress $opt: f.gz was not created"
	set -- $(od -An -tx1 -N3 f.gz)
	{ [ "$1" = 1f ] && [ "$2" = 8b ] && [ "$3" = 08 ]; } || pc_fail "compress $opt: output does not start with the gzip signature and method"
	compress -d f.gz </dev/null >out 2>err
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "compress -d f.gz after $opt: exit status $rc"
	cmp -s f orig || pc_fail "round trip after $opt differs"
done
pc_pass
