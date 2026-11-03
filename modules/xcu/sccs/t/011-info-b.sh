# info -b
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.011
# info -b is accepted and lists nothing that plain info does not, with an edit of a
# trunk SID and of a branch SID present (the page's parenthesis about branches
# contradicts the usual meaning, so only this is required).
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.011
LC_ALL=C; export LC_ALL
unset PROJECTDIR
for u in sccs admin get; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT

# pc_mk NAME TEXT: SCCS/s.NAME holding the one line TEXT as delta 1.1.
pc_mk() {
	printf '%s\n' "$2" >"$1" || return 1
	admin -i"$1" "SCCS/s.$1" </dev/null >/dev/null 2>&1 || return 1
	rm -f "$1"
}
# pc_next NAME TEXT: a further delta whose text is the one line TEXT.
pc_next() {
	get -e "SCCS/s.$1" </dev/null >/dev/null 2>&1 || return 1
	printf '%s\n' "$2" >"$1" || return 1
	delta -y"c" "SCCS/s.$1" </dev/null >/dev/null 2>&1 || return 1
	rm -f "$1"
}
mkdir SCCS || pc_unresolved "setup: mkdir"
pc_mk pctrunk "one" && pc_mk pcbranch "one" && pc_next pcbranch "two" || pc_unresolved "setup: pc_mk"
get -e SCCS/s.pctrunk </dev/null >/dev/null 2>&1 || pc_unresolved "get -e failed"
get -e -r1.1 -b SCCS/s.pcbranch </dev/null >/dev/null 2>&1 || pc_unresolved "get -e -b of an old SID failed"
sccs info </dev/null >all 2>/dev/null || pc_unresolved "sccs info failed"
pc_test
sccs info -b </dev/null >out 2>err || pc_fail "sccs info -b: nonzero status: $(cat err)"
for n in pctrunk pcbranch; do
	if grep $n out >/dev/null 2>&1 && ! grep $n all >/dev/null 2>&1; then
		pc_fail "-b lists $n, which plain info does not"
	fi
done
pc_pass
