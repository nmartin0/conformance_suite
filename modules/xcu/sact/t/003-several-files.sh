# several files and standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sact.003
# With several files each name is written as a heading before its lines.
. "$PC_CORE/pc.sh"
pc_begin XCU.sact.003
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mk1() {
	printf 'one\ntwo\nthree\n' >"$1" || pc_unresolved "setup: cannot write $1"
	admin -i"$1" "s.$1" </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin failed for s.$1"
	rm -f "$1"
}
mk1 a; mk1 b
get -e -s s.a </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
get -e -s s.b </dev/null >setup.out 2>&1 || pc_unresolved "setup: get -e failed"
pc_test
sact s.a s.b </dev/null >out 2>err || pc_fail "sact s.a s.b: nonzero status"
grep -x 's\.a:' out >/dev/null || pc_fail "no s.a: heading: $(cat out)"
grep -x 's\.b:' out >/dev/null || pc_fail "no s.b: heading: $(cat out)"
n=$(grep -c '^1\.1 1\.2 ' out)
[ "$n" -eq 2 ] || pc_fail "expected two pending lines, found $n: $(cat out)"
printf 's.a\ns.b\n' | sact - >out 2>err || pc_fail "sact -: nonzero status"
grep -x 's\.a:' out >/dev/null && grep -x 's\.b:' out >/dev/null || pc_fail "standard input names gave: $(cat out)"
pc_pass
