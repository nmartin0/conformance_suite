# edit
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sccs.009
# edit is equivalent to get -e: same report, same status, a writable g-file.
. "$PC_CORE/pc.sh"
pc_begin XCU.sccs.009
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
for d in t1 t2; do
	mkdir -p "$d/SCCS" || pc_unresolved "setup: mkdir"
	printf 'one\n' >"$d/f" && (cd "$d" && admin -if SCCS/s.f </dev/null >/dev/null 2>&1 && rm -f f) || pc_unresolved "setup: admin in $d"
done
pc_test
(cd t1 && sccs edit f </dev/null >../o1 2>../e1); r1=$?
(cd t2 && get -e SCCS/s.f </dev/null >../o2 2>../e2); r2=$?
[ "$r1" -eq "$r2" ] || pc_fail "status $r1 against $r2 for get -e"
[ "$r2" -eq 0 ] || pc_unresolved "get -e failed here too"
cmp o1 o2 >/dev/null 2>&1 || pc_fail "standard output differs from get -e: $(cat o1)"
cmp e1 e2 >/dev/null 2>&1 || pc_fail "standard error differs from get -e: $(cat e1)"
[ -f t1/f ] || pc_fail "no g-file"
case $(ls -l t1/f | cut -c3) in
w) ;;
*) pc_fail "the g-file is not writable by its owner" ;;
esac
(cd t1 && sccs tell | grep f >/dev/null 2>&1) || pc_fail "f is not recorded as being edited"
pc_pass
