# pax -o: syntax of the options argument
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.036
# Of two "-o comment=" options the later one wins, a backslash-comma keeps
# the comma in the value, a trailing comma (also one followed by blanks) is
# ignored and keywords may be preceded by blanks; a comment record is ignored
# when the archive is read.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.036
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
fld() { dd if="$1" bs=1 skip="$2" count="$3" 2>/dev/null | tr -d '\000'; }
num() { echo $((0$(fld "$1" "$2" "$3"))); }
# gdata <archive>: the records of the first typeflag g block
gdata() {
	o=0; end=$(wc -c <"$1"); end=$((end + 0))
	while [ "$o" -lt "$end" ]; do
		n=$(fld "$1" "$o" 100)
		[ -n "$n" ] || return 1
		t=$(fld "$1" $((o + 156)) 1)
		s=$(num "$1" $((o + 124)) 12)
		if [ "$t" = g ]; then
			dd if="$1" bs=1 skip=$((o + 512)) count="$s" 2>/dev/null
			return 0
		fi
		case $t in 1|2|3|4|5|6) k=0 ;; *) k=$(((s + 511) / 512)) ;; esac
		o=$((o + 512 + 512 * k))
	done
	return 1
}
# try <archive> <-o arguments...>
try() { a=$1; shift; pax -w -x pax "$@" f1 >"$a" 2>err </dev/null || pc_fail "pax -w -x pax $*: nonzero status"; gdata "$a" >"$a.rec" || pc_fail "pax -w -x pax $*: no typeflag g block"; }
pc_test
try a1 -o comment=one -o comment=two
grep -E '^[0-9]+ comment=two$' a1.rec >/dev/null || pc_fail "the later -o comment=two left no such record"
grep one a1.rec >/dev/null && pc_fail "the earlier -o comment=one was not ignored"
[ "$(pax <a1 2>/dev/null)" = f1 ] || pc_fail "an archive with a comment record does not list as f1"
rm -f f1
pax -r <a1 >o 2>err || pc_fail "extracting an archive with a comment record: nonzero status"
[ "$(cat f1 2>/dev/null)" = 12345 ] || pc_fail "the comment record disturbed the extraction of f1"
try a2 -o 'comment=a\,b'
grep -E '^[0-9]+ comment=a,b$' a2.rec >/dev/null || pc_fail "-o 'comment=a\\,b' did not give the value a,b: $(cat a2.rec)"
try a3 -o 'comment=hi,'
grep -E '^[0-9]+ comment=hi$' a3.rec >/dev/null || pc_fail "a trailing comma was not ignored: $(cat a3.rec)"
try a4 -o 'comment=hi, '
grep -E '^[0-9]+ comment=hi$' a4.rec >/dev/null || pc_fail "a trailing comma followed by a blank was not ignored: $(cat a4.rec)"
try a5 -o ' comment=sp'
grep -E '^[0-9]+ comment=sp$' a5.rec >/dev/null || pc_fail "a keyword preceded by a blank was not accepted: $(cat a5.rec)"
try a6 -o 'comment=c1,gname=gg'
grep -E '^[0-9]+ comment=c1$' a6.rec >/dev/null || pc_fail "the first of two comma-separated keywords has no record: $(cat a6.rec)"
grep -E '^[0-9]+ gname=gg$' a6.rec >/dev/null || pc_fail "the second of two comma-separated keywords has no record: $(cat a6.rec)"
pc_pass
