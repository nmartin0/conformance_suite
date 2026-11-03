# -d
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.002
# -d path makes path the root for the SCCS files: the pathname is prefixed with it,
# whether the path is absolute or relative.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.002
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
mkdir -p root/SCCS || pc_unresolved "setup: mkdir"
printf 'root\n' >f && admin -if root/SCCS/s.f </dev/null >/dev/null 2>&1 || pc_unresolved "setup: admin"
rm -f f
pc_test
printf 'root\n' >exp
sccs -d "$PWD/root" get -p f </dev/null >out 2>err || pc_fail "-d absolute: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "-d absolute: wrong text: $(od -c out | head -3)"
sccs -d root get -p f </dev/null >out 2>err || pc_fail "-d relative: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "-d relative: wrong text: $(od -c out | head -3)"
pc_pass
