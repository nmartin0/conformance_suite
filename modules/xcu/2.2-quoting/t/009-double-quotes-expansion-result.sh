# When double quotes enclose a parameter expansion or a command
# substitution, every character of the result keeps its literal value: no
# field splitting, no filename expansion and no further expansion.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.2-quoting.009
. "$PC_CORE/pc.sh"
pc_begin XCU.2.2-quoting.009
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'x=\047a   b  pcq*  $HOME `x`\047\n'
	printf 'printf \047%%s\\n\047 "$x"\n'
	printf 'printf \047%%s\\n\047 "$(printf \047p   q  *\047)"\n'
} >s.sh || pc_fail "cannot write the test script"
: >pcqa || pc_fail "setup failed"
: >pcqb || pc_fail "setup failed"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a   b  pcq*  $HOME `x`\np   q  *\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
