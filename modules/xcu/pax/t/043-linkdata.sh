# pax -x pax -o linkdata: the data of a hard link member is stored too
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.043
# "pax -w -x pax -o linkdata f1 h" with h a hard link to f1 gives an archive
# in which the typeflag 1 member h has the size 6 and is followed by a data
# block and the end of the archive, where without linkdata h has size 0 and
# the end of the archive follows its header.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.043
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
ln f1 h || pc_unresolved "setup: ln failed"
fld() { dd if="$1" bs=1 skip="$2" count="$3" 2>/dev/null | tr -d '\000'; }
num() { echo $((0$(fld "$1" "$2" "$3"))); }
# second <archive>: the offset of the member that is the hard link (typeflag 1), or nothing
second() {
	o=0; end=$(wc -c <"$1"); end=$((end + 0))
	while [ "$o" -lt "$end" ]; do
		n=$(fld "$1" "$o" 100)
		[ -n "$n" ] || return
		t=$(fld "$1" $((o + 156)) 1)
		[ "$t" = 1 ] && { echo "$o"; return; }
		s=$(num "$1" $((o + 124)) 12)
		case $t in 2|3|4|5|6) k=0 ;; *) k=$(((s + 511) / 512)) ;; esac
		o=$((o + 512 + 512 * k))
	done
}
pc_test
pax -w -x pax f1 h >a0 2>err </dev/null || pc_fail "pax -w -x pax: nonzero status"
pax -w -x pax -o linkdata f1 h >a1 2>err </dev/null || pc_fail "pax -w -x pax -o linkdata: nonzero status"
l0=$(second a0)
[ -n "$l0" ] || pc_fail "without linkdata no typeflag 1 member was found"
[ "$(num a0 $((l0 + 124)) 12)" -eq 0 ] || pc_fail "without linkdata the hard link member has size $(num a0 $((l0 + 124)) 12), expected 0"
l1=$(second a1)
[ -n "$l1" ] || pc_fail "with linkdata no typeflag 1 member was found"
[ "$(num a1 $((l1 + 124)) 12)" -eq 6 ] || pc_fail "with linkdata the hard link member has size $(num a1 $((l1 + 124)) 12), expected 6"
zero() { dd if="$1" bs=1 skip="$2" count=1024 2>/dev/null | od -An -v -tx1 | tr -d ' 0\n'; }
[ -z "$(zero a0 $((l0 + 512)))" ] && [ "$(wc -c <a0)" -ge $((l0 + 1536)) ] || pc_fail "without linkdata the two zero blocks do not follow the hard link header directly"
[ -z "$(zero a1 $((l1 + 1024)))" ] && [ "$(wc -c <a1)" -ge $((l1 + 2048)) ] || pc_fail "with linkdata the two zero blocks do not follow one data block after the hard link header"
[ "$(dd if=a1 bs=1 skip=$((l1 + 512)) count=6 2>/dev/null)" = 12345 ] || pc_fail "the block after the hard link member does not hold the file data"
pc_pass
