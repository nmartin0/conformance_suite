# create
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.008
# create makes the SCCS file from the file of the same name, passes admin options
# (-r2 makes the first SID 2.1), and renames the original with a leading comma.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.008
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
printf 'alpha\nbeta\n' >f || pc_unresolved "setup: write f"
printf 'gamma\n' >g || pc_unresolved "setup: write g"
pc_test
sccs create f </dev/null >out 2>err || pc_fail "sccs create f: nonzero status: $(cat err)"
[ -f SCCS/s.f ] || pc_fail "SCCS/s.f was not made"
[ -f ,f ] || pc_fail "the original was not renamed to ,f"
[ -e f ] && pc_fail "f is still there next to ,f"
printf 'alpha\nbeta\n' >exp
cmp ,f exp >/dev/null 2>&1 || pc_fail ",f does not hold the original text"
get -p SCCS/s.f </dev/null >out 2>err || pc_fail "get -p SCCS/s.f: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "the SCCS file holds other text: $(od -c out | head -3)"
sccs create -r2 g </dev/null >out 2>err || pc_fail "sccs create -r2 g: nonzero status: $(cat err)"
get -p SCCS/s.g </dev/null >out 2>err || pc_fail "get -p SCCS/s.g: nonzero status"
grep -x '2\.1' err >/dev/null 2>&1 || pc_fail "-r2 was not passed to admin (no SID 2.1): $(cat err)"
[ -f ,g ] || pc_fail "the original g was not renamed to ,g"
pc_pass
