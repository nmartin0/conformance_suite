# default SCCS directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.001
# With no options sccs prefixes an operand with the SCCS directory and the s. prefix,
# runs the named utility on it, and passes options through, so sccs get -p gives what
# get -p gives on SCCS/s.f: the same text, status and standard error, and no g-file.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.001
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
pc_mk f "one" || pc_unresolved "setup: cannot make SCCS/s.f"
get -p SCCS/s.f </dev/null >exp.out 2>exp.err || pc_unresolved "setup: get -p SCCS/s.f failed"
printf 'one\n' >exp.txt
cmp exp.out exp.txt >/dev/null 2>&1 || pc_unresolved "setup: unexpected text from get"
pc_test
sccs get -p f </dev/null >out 2>err || pc_fail "sccs get -p f: nonzero status"
cmp out exp.out >/dev/null 2>&1 || pc_fail "standard output differs from get -p: $(od -c out | head -3)"
cmp err exp.err >/dev/null 2>&1 || pc_fail "standard error differs from get -p: $(cat err)"
[ -e f ] && pc_fail "a g-file was created"
pc_pass
