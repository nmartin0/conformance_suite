# PROJECTDIR absolute
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.005
# PROJECTDIR is the default for -d: a value beginning with a slash is an absolute
# pathname used as the root of the SCCS files.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.005
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
mkdir -p other/SCCS || pc_unresolved "setup: mkdir"
printf 'other\n' >f && admin -if other/SCCS/s.f </dev/null >/dev/null 2>&1 || pc_unresolved "setup: admin other"
rm -f f
pc_test
PROJECTDIR=$PWD/other; export PROJECTDIR
sccs get -p f </dev/null >out 2>err || pc_fail "nonzero status"
printf 'other\n' >exp
cmp out exp >/dev/null 2>&1 || pc_fail "PROJECTDIR was not used as the root: $(od -c out | head -3) $(cat err)"
pc_pass
