# pax -x ustar: header fields and type flags for the kinds of file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.032
# In a ustar archive of a file, a hard link to it, a directory, a symbolic
# link and a FIFO, the header fields hold the uid, gid, mtime, uname and
# gname of the file, the type flags are 0, 1 (with the first name in
# linkname and size 0), 5, 2 (with the target in linkname and size 0) and 6,
# and no data records follow the headers of types 1, 2, 5 and 6 (the two
# zero blocks follow the last header); the mode field of a file with mode
# 6777 is 6777.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.032
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'umask 022; cd "$pc_top" && rm -rf "$pc_wd"' EXIT
umask 022
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
chmod 640 f1 && touch -d 2003-01-02T15:04:05Z f1 || pc_unresolved "setup: chmod or touch failed"
ln f1 h || pc_unresolved "setup: ln failed"
mkdir d && chmod 755 d || pc_unresolved "setup: mkdir failed"
ln -s f1 s || pc_unresolved "setup: ln -s failed"
mkfifo p || pc_unresolved "setup: mkfifo failed"
printf 'm\n' >m && chmod 6777 m || pc_unresolved "setup: cannot make m with mode 6777"
[ "$(ls -l m | cut -c1-10)" = '-rwsrwsrwx' ] || pc_unresolved "setup: the set-id bits of m could not be set"
set -- $(ls -ln f1)
uid=$3; gid=$4
set -- $(ls -l f1)
uname=$3; gname=$4
fld() { dd if="$1" bs=1 skip="$2" count="$3" 2>/dev/null | tr -d '\000'; }
num() { echo $((0$(fld "$1" "$2" "$3"))); }
# scan <archive>: scan.out gets "offset typeflag name size" per header; endoff
# is the offset of the first block with an empty name.
scan() {
	o=0; end=$(wc -c <"$1"); end=$((end + 0)); : >scan.out; endoff=
	while [ "$o" -lt "$end" ]; do
		n=$(fld "$1" "$o" 100)
		if [ -z "$n" ]; then endoff=$o; break; fi
		t=$(fld "$1" $((o + 156)) 1)
		s=$(num "$1" $((o + 124)) 12)
		n=${n%/}
		echo "$o $t $n $s" >>scan.out
		case $t in 1|2|3|4|5|6) k=0 ;; *) k=$(((s + 511) / 512)) ;; esac
		o=$((o + 512 + 512 * k))
	done
}
# ent <name>: "typeflag size offset"
ent() { awk -v n="$1" '$3 == n { print $2 " " $4 " " $1; exit }' scan.out; }
pc_test
pax -w -x ustar f1 h d s p m >a 2>err </dev/null || pc_fail "pax -w -x ustar: nonzero status"
scan a
[ -n "$endoff" ] || pc_fail "walking the headers by their sizes did not reach a block with an empty name"
sz=$(wc -c <a); sz=$((sz + 0))
[ $((sz % 512)) -eq 0 ] && [ "$sz" -ge $((endoff + 1024)) ] || pc_fail "the archive ($sz octets) lacks the two zero blocks after the last header (at $endoff)"
z=$(dd if=a bs=1 skip="$endoff" count=1024 2>/dev/null | od -An -v -tx1 | tr -d ' 0\n')
[ -z "$z" ] || pc_fail "the two blocks after the last header are not zero"
[ "$(wc -l <scan.out | tr -d ' ')" -eq 6 ] || pc_fail "the archive holds $(wc -l <scan.out | tr -d ' ') headers, expected 6 (type 1, 2, 5 and 6 headers take no data blocks)"
set -- $(ent f1); tf1=$1; sf1=$2; of1=$3
set -- $(ent h); th=$1; sh_=$2; oh=$3
[ -n "$tf1" ] && [ -n "$th" ] || pc_fail "f1 or h is missing from the archive"
if [ "$tf1" = 0 ] && [ "$th" = 1 ]; then
	data=f1; od_=$of1; link=h; ol=$oh; first=f1; sl=$sh_; sd=$sf1
elif [ "$tf1" = 1 ] && [ "$th" = 0 ]; then
	data=h; od_=$oh; link=f1; ol=$of1; first=h; sl=$sf1; sd=$sh_
else
	pc_fail "the type flags of f1 and h are '$tf1' and '$th', expected one 0 and one 1"
fi
[ "$sl" -eq 0 ] || pc_fail "the hard link member $link has size $sl, expected 0"
[ "$sd" -eq 6 ] || pc_fail "the file member $data has size $sd, expected 6"
[ "$(fld a $((ol + 157)) 100)" = "$first" ] || pc_fail "the linkname of $link is '$(fld a $((ol + 157)) 100)', expected $first"
[ "$(num a $((od_ + 108)) 8)" -eq "$uid" ] || pc_fail "uid field '$(fld a $((od_ + 108)) 8)' does not match the owner $uid"
[ "$(num a $((od_ + 116)) 8)" -eq "$gid" ] || pc_fail "gid field '$(fld a $((od_ + 116)) 8)' does not match the group $gid"
[ "$(num a $((od_ + 136)) 12)" -eq 1041519845 ] || pc_fail "mtime field '$(fld a $((od_ + 136)) 12)' is not the octal form of 1041519845"
case $uname in
''|*[!0-9]*) if [ ${#uname} -le 31 ]; then
		[ "$(fld a $((od_ + 265)) 32)" = "$uname" ] || pc_fail "uname field is '$(fld a $((od_ + 265)) 32)', expected $uname"
	fi ;;
esac
case $gname in
''|*[!0-9]*) if [ ${#gname} -le 31 ]; then
		[ "$(fld a $((od_ + 297)) 32)" = "$gname" ] || pc_fail "gname field is '$(fld a $((od_ + 297)) 32)', expected $gname"
	fi ;;
esac
set -- $(ent d)
[ "$1" = 5 ] || pc_fail "the directory d has type flag '$1', expected 5"
[ $(( $(num a $(($3 + 100)) 8) & 511 )) -eq 493 ] || pc_fail "the mode field of d is '$(fld a $(($3 + 100)) 8)', expected permission bits 755"
set -- $(ent s)
[ "$1" = 2 ] || pc_fail "the symbolic link s has type flag '$1', expected 2"
[ "$2" -eq 0 ] || pc_fail "the symbolic link s has size $2, expected 0"
[ "$(fld a $(($3 + 157)) 100)" = f1 ] || pc_fail "the linkname of s is '$(fld a $(($3 + 157)) 100)', expected f1"
set -- $(ent m)
[ -n "$3" ] && [ $(( $(num a $(($3 + 100)) 8) & 4095 )) -eq $((06777)) ] || pc_fail "the mode field of m is '$(fld a $(($3 + 100)) 8)', expected 6777"
set -- $(ent p)
[ "$1" = 6 ] || pc_fail "the FIFO p has type flag '$1', expected 6"
pc_pass
