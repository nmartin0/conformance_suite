# fix
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.021
# fix -r SID removes the delta but leaves its text as the file in the directory.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.021
LC_ALL=C; export LC_ALL
unset PROJECTDIR
for u in sccs admin get delta prs rmdel; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
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
pc_mk f "base" && pc_next f "second" || pc_unresolved "setup: pc_mk"
prs -e -d:I: SCCS/s.f </dev/null 2>/dev/null | grep '1\.2' >/dev/null || pc_unresolved "setup: delta 1.2 not listed"
pc_test
sccs fix -r1.2 f </dev/null >out 2>err || pc_fail "sccs fix -r1.2: nonzero status: $(cat out) $(cat err)"
prs -e -d:I: SCCS/s.f </dev/null >sids 2>/dev/null
grep '1\.2' sids >/dev/null 2>&1 && pc_fail "delta 1.2 is still there: $(cat sids)"
[ -f f ] || pc_fail "no copy of the delta was left"
printf 'second\n' >exp
cmp f exp >/dev/null 2>&1 || pc_fail "the left copy is not the text of the removed delta: $(od -c f | head -3)"
get -p -r1.1 SCCS/s.f </dev/null >out 2>err || pc_fail "delta 1.1 is gone"
printf 'base\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "delta 1.1 changed"
pc_pass
