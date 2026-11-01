# delget
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.017
# delget makes a delta and then gets the new version with keywords expanded and
# not for editing.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.017
LC_ALL=C; export LC_ALL
unset PROJECTDIR
for u in sccs admin get delta; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
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
pc_mk f "base" || pc_unresolved "setup: pc_mk"
get -e SCCS/s.f </dev/null >/dev/null 2>&1 || pc_unresolved "get -e failed"
printf 'line %s\n' '%M%' >f || pc_unresolved "setup: write"
pc_test
sccs delget -y"pc comment" f </dev/null >out 2>err || pc_fail "sccs delget: nonzero status: $(cat err)"
[ -f f ] || pc_fail "no g-file after delget"
printf 'line f\n' >exp
cmp f exp >/dev/null 2>&1 || pc_fail "the new version is not the delta with keywords expanded: $(od -c f | head -3)"
case $(ls -l f | cut -c3) in
w) pc_fail "the new version is editable" ;;
esac
sccs check </dev/null >out 2>err || pc_fail "the file is still recorded as being edited: $(cat out)"
get -p -r1.2 SCCS/s.f </dev/null >out 2>err || pc_fail "no delta 1.2 was made"
pc_pass
