# In filename expansion a star or question mark does not match a leading
# period, so a leading period needs an explicit period in the pattern, and a
# slash is matched only by a slash in the pattern.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.004
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.004
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo pcd1/*\n'
	printf 'echo pcd1/.h*\n'
	printf 'echo pcd1*x\n'
	printf 'echo pcd*/v\n'
} >s.sh || pc_fail "cannot write the test script"
mkdir pcd1 || pc_fail "setup failed"
: >pcd1/.h || pc_fail "setup failed"
: >pcd1/v || pc_fail "setup failed"
: >pcd1x || pc_fail "setup failed"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'pcd1/v\npcd1/.h\npcd1x\npcd1/v\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
