# pax -x pax: extended header blocks, their names, and the records in them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.035
# With -x pax, "-o comment:=hello" puts a typeflag x block holding the
# record "17 comment=hello" before the file; "-o comment=hello" puts a
# typeflag g block with it first; the g block is named
# $TMPDIR/GlobalHead.<pid>.1 by default (/tmp when TMPDIR is unset), the x
# block ./PaxHeaders.<pid>/f1, and -o globexthdr.name and -o exthdr.name
# change those names with %n, %p, %d, %f and %% replaced; -o delete=?time
# removes the atime and mtime records that -o times writes; the mtime
# record holds the file's time in seconds since the Epoch.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.035
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
touch -d 2003-01-02T15:04:05Z f1 || pc_unresolved "setup: touch -d failed"
mkdir tmpd || pc_unresolved "setup: mkdir failed"
TD=$(pwd)/tmpd
[ ${#TD} -le 60 ] || TD=/pcxtmp.$$
fld() { dd if="$1" bs=1 skip="$2" count="$3" 2>/dev/null | tr -d '\000'; }
num() { echo $((0$(fld "$1" "$2" "$3"))); }
# scan <archive>: scan.out gets "offset typeflag name size" per header
scan() {
	o=0; end=$(wc -c <"$1"); end=$((end + 0)); : >scan.out; endoff=
	while [ "$o" -lt "$end" ]; do
		n=$(fld "$1" "$o" 100)
		if [ -z "$n" ]; then endoff=$o; break; fi
		t=$(fld "$1" $((o + 156)) 1)
		s=$(num "$1" $((o + 124)) 12)
		echo "$o $t $n $s" >>scan.out
		case $t in 1|2|3|4|5|6) k=0 ;; *) k=$(((s + 511) / 512)) ;; esac
		o=$((o + 512 + 512 * k))
	done
}
# first header with a typeflag: "offset name size"
hdr() { awk -v t="$1" '$2 == t { print $1 " " $3 " " $4; exit }' scan.out; }
# data <archive> <header offset> <size>
data() { dd if="$1" bs=1 skip=$(($2 + 512)) count="$3" 2>/dev/null; }
recok() { data "$@" | awk '{ if ($1 != length($0) + 1 || index($0, "=") == 0) bad = 1 } END { exit bad + 0 }'; }
pc_test
pax -w -x pax -o comment:=hello f1 >a1 2>err </dev/null || pc_fail "-o comment:=hello: nonzero status"
scan a1
set -- $(hdr x)
[ -n "$1" ] || pc_fail "-o comment:=hello wrote no typeflag x block"
xo=$1; xn=$2; xs=$3
[ -z "$(hdr g)" ] || pc_fail "-o comment:=hello wrote a typeflag g block"
[ "$xs" -ge 17 ] || pc_fail "the typeflag x block has size $xs, too small for the record"
[ "$(data a1 "$xo" 17)" = "17 comment=hello" ] || pc_fail "the x block does not start with the record '17 comment=hello': '$(data a1 "$xo" 17)'"
recok a1 "$xo" "$xs" || pc_fail "a record in the x block has a length that does not match its octets"
printf '%s\n' "$xn" | grep -E '^\./PaxHeaders\.[0-9]+/f1$' >/dev/null || pc_fail "the x block is named '$xn', expected ./PaxHeaders.<pid>/f1"
set -- $(hdr 0)
[ "$2" = f1 ] || pc_fail "the file header after the x block names '$2', expected f1"
z=$(dd if=a1 bs=1 skip="$endoff" count=1024 2>/dev/null | od -An -v -tx1 | tr -d ' 0\n')
[ -n "$endoff" ] && [ -z "$z" ] && [ "$(wc -c <a1)" -ge $((endoff + 1024)) ] || pc_fail "the two zero blocks do not follow the last file in the pax archive"
pax -w -x pax -o 'exthdr.name=%d/XH.%f' -o comment:=hello f1 >a2b 2>err </dev/null || pc_fail "-o exthdr.name with %d: nonzero status"
scan a2b
set -- $(hdr x)
[ "$2" = './XH.f1' ] || pc_fail "with -o exthdr.name='%d/XH.%f' the x block is named '$2', expected ./XH.f1"
pax -w -x pax -o 'exthdr.name=XH/%f.%%' -o comment:=hello f1 >a2 2>err </dev/null || pc_fail "-o exthdr.name: nonzero status"
scan a2
set -- $(hdr x)
[ "$2" = 'XH/f1.%' ] || pc_fail "with -o exthdr.name='XH/%f.%%' the x block is named '$2', expected XH/f1.%"
( unset TMPDIR; pax -w -x pax -o comment=hello f1 >a3 2>err </dev/null ) || pc_fail "-o comment=hello: nonzero status"
scan a3
set -- $(hdr g)
[ -n "$1" ] || pc_fail "-o comment=hello wrote no typeflag g block"
go=$1; gn=$2; gs=$3
head -n 1 scan.out | grep ' g ' >/dev/null || pc_fail "the typeflag g block is not the first block of the archive"
[ "$gs" -ge 17 ] || pc_fail "the typeflag g block has size $gs, too small for the record"
[ "$(data a3 "$go" 17)" = "17 comment=hello" ] || pc_fail "the g block does not hold the record '17 comment=hello'"
printf '%s\n' "$gn" | grep -E '^/tmp/GlobalHead\.[0-9]+\.1$' >/dev/null || pc_fail "with TMPDIR unset the g block is named '$gn', expected /tmp/GlobalHead.<pid>.1"
( unset TMPDIR; pax -w -x pax -o comment=hello f1 >a4 2>err </dev/null ) || pc_fail "-o comment=hello without TMPDIR: nonzero status"
scan a4
set -- $(hdr g)
printf '%s\n' "$2" | grep -E '^/tmp/GlobalHead\.[0-9]+\.1$' >/dev/null || pc_fail "with TMPDIR unset the g block is named '$2', expected /tmp/GlobalHead.<pid>.1"
TMPDIR=$TD pax -w -x pax -o comment=hello f1 >a5 2>err </dev/null || pc_fail "-o comment=hello with TMPDIR set: nonzero status"
scan a5
set -- $(hdr g)
printf '%s\n' "$2" | grep -E "^$TD/GlobalHead\\.[0-9]+\\.1\$" >/dev/null || pc_fail "with TMPDIR=$TD the g block is named '$2'"
pax -w -x pax -o 'globexthdr.name=GH.%n.%p.%%' -o comment=hello f1 >a6 2>err </dev/null || pc_fail "-o globexthdr.name: nonzero status"
scan a6
set -- $(hdr g)
printf '%s\n' "$2" | grep -E '^GH\.1\.[0-9]+\.%$' >/dev/null || pc_fail "with -o globexthdr.name='GH.%n.%p.%%' the g block is named '$2'"
pax -w -x pax -o times f1 >a7 2>err </dev/null || pc_fail "-o times: nonzero status"
scan a7
set -- $(hdr x)
[ -n "$1" ] || pc_fail "-o times wrote no typeflag x block"
recok a7 "$1" "$3" || pc_fail "a record of the -o times block has a length that does not match its octets"
data a7 "$1" "$3" | grep -E '^[0-9]+ mtime=1041519845(\.0*)?$' >/dev/null || pc_fail "no mtime record holding 1041519845 seconds since the Epoch"
data a7 "$1" "$3" | grep -E '^[0-9]+ atime=[0-9]+(\.[0-9]+)?$' >/dev/null || pc_fail "no atime record holding a time in seconds since the Epoch"
pax -w -x pax -o times -o 'delete=?time' f1 >a8 2>err </dev/null || pc_fail "-o delete: nonzero status"
tr -d '\000' <a8 | grep -E 'atime=|mtime=' >/dev/null && pc_fail "-o delete=?time left an atime or mtime record in the archive"
pax -w -x pax -o times -o delete=atime f1 >a9 2>err </dev/null || pc_fail "-o delete=atime: nonzero status"
tr -d '\000' <a9 | grep 'atime=' >/dev/null && pc_fail "-o delete=atime left an atime record in the archive"
tr -d '\000' <a9 | grep 'mtime=' >/dev/null || pc_fail "-o delete=atime removed the mtime record as well"
pc_pass
