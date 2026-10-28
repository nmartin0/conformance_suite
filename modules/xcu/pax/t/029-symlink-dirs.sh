# pax -H and -L with a symbolic link to a directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.029
# For a tree t holding a symbolic link ld to a directory d: by default and
# with -H the link ld is archived as a link only; with -L the files below it
# (t/ld/x) are archived too and t/ld is a directory; "-H t/ld" on the
# command line archives t/ld as a directory with t/ld/x; with both -H and -L
# the last one decides.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.029
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir t d || pc_unresolved "setup: mkdir failed"
printf 'x\n' >d/x || pc_unresolved "setup: cannot write d/x"
ln -s ../d t/ld || pc_unresolved "setup: ln -s failed"
names() { pax <"$1" 2>/dev/null | sort | tr '\n' ' '; }
# type_of <archive> <name>: first character of the verbose line for the name
type_of() { pax -v <"$1" 2>/dev/null | awk -v n="$2" '{ for (i = 1; i <= NF; i++) if ($i == n) { print substr($1, 1, 1); exit } }'; }
# build <archive> <options> <operand>
build() { a=$1; shift; pax -w "$@" >"$a" 2>err </dev/null || pc_fail "pax -w $*: nonzero status"; }
pc_test
for opts in "" "-H"; do
	build a.tmp $opts t
	[ "$(names a.tmp)" = 't t/ld ' ] || pc_fail "pax -w $opts t lists '$(names a.tmp)', expected 't t/ld '"
	[ "$(type_of a.tmp t/ld)" = l ] || pc_fail "pax -w $opts t: t/ld is not archived as a symbolic link"
done
build a.l -L t
[ "$(names a.l)" = 't t/ld t/ld/x ' ] || pc_fail "pax -w -L t lists '$(names a.l)', expected 't t/ld t/ld/x '"
[ "$(type_of a.l t/ld)" = d ] || pc_fail "pax -w -L t: t/ld is not archived as a directory"
build a.hl -H t/ld
[ "$(names a.hl)" = 't/ld t/ld/x ' ] || pc_fail "pax -w -H t/ld lists '$(names a.hl)', expected 't/ld t/ld/x '"
[ "$(type_of a.hl t/ld)" = d ] || pc_fail "pax -w -H t/ld: t/ld is not archived as a directory"
build a.plain t/ld
[ "$(names a.plain)" = 't/ld ' ] || pc_fail "pax -w t/ld lists '$(names a.plain)', expected the link alone"
[ "$(type_of a.plain t/ld)" = l ] || pc_fail "pax -w t/ld: the link is not archived as a symbolic link"
build a.hlopt -H -L t
[ "$(names a.hlopt)" = 't t/ld t/ld/x ' ] || pc_fail "pax -w -H -L t lists '$(names a.hlopt)': the last option, -L, did not decide"
build a.lhopt -L -H t
[ "$(names a.lhopt)" = 't t/ld ' ] || pc_fail "pax -w -L -H t lists '$(names a.lhopt)': the last option, -H, did not decide"
pc_pass
