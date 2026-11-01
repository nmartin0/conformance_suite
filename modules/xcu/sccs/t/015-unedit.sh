# unedit
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.015
# unedit undoes edit: the file is no longer being edited, the changes made since
# the get are gone, and it can be edited again.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.015
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
sccs edit pcfile </dev/null >/dev/null 2>&1 || pc_unresolved "sccs edit failed"
printf 'changed\n' >pcfile || pc_unresolved "setup: write"
pc_test
sccs unedit pcfile </dev/null >out 2>err || pc_fail "sccs unedit: nonzero status: $(cat err)"
sccs check </dev/null >out 2>err || pc_fail "the file is still being edited: $(cat out)"
if [ -e pcfile ]; then
	grep changed pcfile >/dev/null 2>&1 && pc_fail "the changes were not discarded"
fi
sccs edit pcfile </dev/null >out 2>err || pc_fail "cannot edit again: $(cat err)"
pc_pass
