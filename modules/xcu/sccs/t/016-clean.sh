# clean
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.016
# clean removes what can be recreated from SCCS files but not a file being edited.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.016
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
pc_mk pcgot "one" && pc_mk pcedit "two" || pc_unresolved "setup: pc_mk"
get SCCS/s.pcgot </dev/null >/dev/null 2>&1 || pc_unresolved "get failed"
get -e SCCS/s.pcedit </dev/null >/dev/null 2>&1 || pc_unresolved "get -e failed"
[ -f pcgot ] && [ -f pcedit ] || pc_unresolved "setup: g-files missing"
pc_test
sccs clean </dev/null >out 2>err || pc_fail "sccs clean: nonzero status: $(cat err)"
[ -e pcgot ] && pc_fail "a g-file that can be recreated was not removed"
[ -f pcedit ] || pc_fail "the g-file being edited was removed"
printf 'two\n' >exp
cmp pcedit exp >/dev/null 2>&1 || pc_fail "the g-file being edited was altered"
pc_pass
