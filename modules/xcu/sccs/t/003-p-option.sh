# -p
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.003
# -p path replaces the SCCS directory name, inserted before the last component:
# -p y get a/b reads a/y/s.b, and with -d /x it reads /x/a/y/s.b.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.003
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
mkdir -p a/y root/a/y || pc_unresolved "setup: mkdir"
printf 'plain\n' >b && admin -ib a/y/s.b </dev/null >/dev/null 2>&1 || pc_unresolved "setup: admin a/y/s.b"
printf 'rooted\n' >b && admin -ib root/a/y/s.b </dev/null >/dev/null 2>&1 || pc_unresolved "setup: admin root/a/y/s.b"
rm -f b
pc_test
printf 'plain\n' >exp
sccs -p y get -p a/b </dev/null >out 2>err || pc_fail "-p y get a/b: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "-p y get a/b: wrong text: $(od -c out | head -3)"
printf 'rooted\n' >exp
sccs -d "$PWD/root" -p y get -p a/b </dev/null >out 2>err || pc_fail "-d root -p y get a/b: nonzero status"
cmp out exp >/dev/null 2>&1 || pc_fail "-d root -p y get a/b: wrong text: $(od -c out | head -3)"
pc_pass
