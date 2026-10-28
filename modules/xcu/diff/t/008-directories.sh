# With two directories diff reports files present in only one, compares common files, and with -r descends into common subdirectories; a file against a directory compares it with the same-named file there.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.diff.008
. "$PC_CORE/pc.sh"
pc_begin XCU.diff.008
LC_ALL=POSIX
export LC_ALL
NL='
'
mkf() {
	n=$1
	shift
	: >"$n" || pc_unresolved "cannot create input"
	for l in "$@"; do printf '%s\n' "$l" >>"$n"; done
}
mkdir d1 d2 d1/sub d2/sub d3 d4 || pc_unresolved "cannot create directories"
printf 'x\n' >d1/only1 && printf 'y\n' >d2/only2 || pc_unresolved "cannot create input"
printf 'same\n' >d1/same && printf 'same\n' >d2/same || pc_unresolved "cannot create input"
printf 'one\n' >d1/dif && printf 'two\n' >d2/dif || pc_unresolved "cannot create input"
printf 'p\n' >d1/sub/f && printf 'q\n' >d2/sub/f && printf 'z\n' >d2/sub/z || pc_unresolved "cannot create input"
printf 'k\n' >d3/k && printf 'k\n' >d4/k || pc_unresolved "cannot create input"
pc_test
out=$(diff d1 d2 2>/dev/null </dev/null)
rc=$?
[ "$rc" -eq 1 ] || pc_fail "two different directories did not give status 1"
case "$NL$out$NL" in
*"${NL}Only in d1: only1${NL}"*) ;;
*) pc_fail "no 'Only in d1: only1' line" ;;
esac
case "$NL$out$NL" in
*"${NL}Only in d2: only2${NL}"*) ;;
*) pc_fail "no 'Only in d2: only2' line" ;;
esac
case "$NL$out$NL" in
*"${NL}diff "*dif*) ;;
*) pc_fail "no 'diff ...' line for the differing common file" ;;
esac
case $out in
*same*) pc_fail "the identical common file same was mentioned" ;;
esac
case $out in
*"Only in d2/sub"*) pc_fail "without -r, diff went into the common subdirectory" ;;
esac
out=$(diff -r d1 d2 2>/dev/null </dev/null)
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-r on different directories did not give status 1"
case "$NL$out$NL" in
*"${NL}Only in d1: only1${NL}"*) ;;
*) pc_fail "-r: no 'Only in d1: only1' line" ;;
esac
case "$NL$out$NL" in
*"${NL}Only in d2/sub: z${NL}"*) ;;
*) pc_fail "-r: no 'Only in d2/sub: z' line" ;;
esac
case "$NL$out$NL" in
*"${NL}1c1${NL}< p${NL}---${NL}> q${NL}"*) ;;
*) pc_fail "-r: the difference in sub/f was not shown" ;;
esac
case "$NL$out$NL" in
*"${NL}1c1${NL}< one${NL}---${NL}> two${NL}"*) ;;
*) pc_fail "-r: the difference in dif was not shown" ;;
esac
case $out in
*same*) pc_fail "-r: the identical file same was mentioned" ;;
esac
got=$(diff -r d3 d4 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "-r on identical directories wrote output or gave a status other than 0"
got=$(diff d1/same d2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "rc=0" ] || pc_fail "a file against a directory holding an identical file of that name wrong"
got=$(diff d1/dif d2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1c1${NL}< one${NL}---${NL}> two${NL}rc=1" ] || pc_fail "a file against a directory holding a file of that name wrong"
got=$(diff d2 d1/dif 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1c1${NL}< two${NL}---${NL}> one${NL}rc=1" ] || pc_fail "a directory against a file wrong"
pc_pass
