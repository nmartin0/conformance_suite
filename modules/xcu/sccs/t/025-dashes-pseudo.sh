# -- after a pseudo-utility
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.025
# The pseudo-utilities follow the syntax guidelines: -- after the command ends its
# options, so sccs edit -- f works like sccs edit f and sccs info -- succeeds.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.025
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
pc_mk f "one" || pc_unresolved "setup: pc_mk"
pc_test
sccs edit -- f </dev/null >out 2>err || pc_fail "sccs edit -- f: nonzero status: $(cat err)"
[ -f f ] || pc_fail "no g-file"
case $(ls -l f | cut -c3) in
w) ;;
*) pc_fail "the g-file is not editable" ;;
esac
sccs info -- </dev/null >out 2>err || pc_fail "sccs info --: nonzero status: $(cat err)"
grep -w f out >/dev/null 2>&1 || pc_fail "info -- does not list the file being edited: $(cat out)"
pc_pass
