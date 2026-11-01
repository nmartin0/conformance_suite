# info
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.010
# info lists the files being edited, and not those that are not.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.010
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
pc_test
sccs info </dev/null >out0 2>err0
grep pcfile out0 >/dev/null 2>&1 && pc_fail "pcfile listed before it is edited: $(cat out0)"
get -e SCCS/s.pcfile </dev/null >/dev/null 2>&1 || pc_unresolved "get -e failed"
sccs info </dev/null >out 2>err || pc_fail "sccs info: nonzero status: $(cat err)"
grep pcfile out >/dev/null 2>&1 || pc_fail "the file being edited is not listed: $(cat out)"
grep pcother out >/dev/null 2>&1 && pc_fail "a file not being edited is listed: $(cat out)"
pc_pass
