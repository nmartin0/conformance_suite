# info -u and -U
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.012
# info -u user lists only files being edited by that user; -U means -u of the
# current user.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.012
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
pc_mk pcfile "one" || pc_unresolved "setup: pc_mk"
get -e SCCS/s.pcfile </dev/null >/dev/null 2>&1 || pc_unresolved "get -e failed"
me=$(id -un) || pc_unresolved "setup: id -un"
pc_test
sccs info -u "$me" </dev/null >out 2>err || pc_fail "info -u $me: nonzero status: $(cat err)"
grep pcfile out >/dev/null 2>&1 || pc_fail "-u $me does not list the file this user is editing: $(cat out)"
sccs info -U </dev/null >out 2>err || pc_fail "info -U: nonzero status: $(cat err)"
grep pcfile out >/dev/null 2>&1 || pc_fail "-U does not list the file the current user is editing: $(cat out)"
sccs info -u pcnobody$$ </dev/null >out 2>err
grep pcfile out >/dev/null 2>&1 && pc_fail "-u of another user lists the file: $(cat out)"
pc_pass
