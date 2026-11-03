# tell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.014
# tell writes the names of the files being edited to standard output and takes
# -u and -U as info does.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.014
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
pc_mk pcfile "one" && pc_mk pcother "two" || pc_unresolved "setup: pc_mk"
get -e SCCS/s.pcfile </dev/null >/dev/null 2>&1 || pc_unresolved "get -e failed"
me=$(id -un) || pc_unresolved "setup: id -un"
pc_test
sccs tell </dev/null >out 2>err || pc_fail "sccs tell: nonzero status: $(cat err)"
grep pcfile out >/dev/null 2>&1 || pc_fail "the file being edited is not listed: $(cat out)"
grep pcother out >/dev/null 2>&1 && pc_fail "a file not being edited is listed: $(cat out)"
sccs tell -u "$me" </dev/null >out 2>err || pc_fail "tell -u: nonzero status"
grep pcfile out >/dev/null 2>&1 || pc_fail "tell -u $me does not list the file: $(cat out)"
sccs tell -U </dev/null >out 2>err || pc_fail "tell -U: nonzero status"
grep pcfile out >/dev/null 2>&1 || pc_fail "tell -U does not list the file: $(cat out)"
sccs tell -u pcnobody$$ </dev/null >out 2>err
grep pcfile out >/dev/null 2>&1 && pc_fail "tell -u of another user lists the file: $(cat out)"
pc_pass
