# diffs
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.020
# diffs writes the differences between the file being edited and the SCCS version;
# -C is passed to diff as -c (context format).
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.020
LC_ALL=C; export LC_ALL
unset PROJECTDIR
for u in sccs admin get diff; do command -v $u >/dev/null 2>&1 || pc_notinuse "$u is not available"; done
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
pc_mk f "one" || pc_unresolved "setup: pc_mk"
get -e SCCS/s.f </dev/null >/dev/null 2>&1 || pc_unresolved "get -e failed"
printf 'one\npcadded\n' >f || pc_unresolved "setup: write"
pc_test
sccs diffs f </dev/null >out 2>err
grep pcadded out >/dev/null 2>&1 || pc_fail "the difference is not listed: $(cat out) $(cat err)"
grep '^\*\*\*' out >/dev/null 2>&1 && pc_fail "plain diffs is in context format"
sccs diffs -C f </dev/null >out 2>err
grep pcadded out >/dev/null 2>&1 || pc_fail "-C: the difference is not listed: $(cat out) $(cat err)"
grep '^\*\*\*' out >/dev/null 2>&1 || pc_fail "-C did not give a context difference: $(cat out)"
pc_pass
