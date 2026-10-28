# ${p:=w} assigns w to p when p is unset or null and ${p=w} only when p is
# unset; in all cases the final value of p is the result, and the assigned
# value has had quote removal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.012
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.012
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'unset x; echo "${x:=a}" "$x"\n'
	printf 'x=; echo "${x:=b}" "$x"\n'
	printf 'x=; echo "[${x=c}]" "[$x]"\n'
	printf 'unset x; echo "${x=d}" "$x"\n'
	printf 'x=v; echo "${x:=e}" "$x"\n'
	printf 'unset y; : ${y:=\047p  q\047}; echo "[$y]"\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'a a\nb b\n[] []\nd d\nv v\n[p  q]\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
