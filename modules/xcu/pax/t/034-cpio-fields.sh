# pax -x cpio: header fields and special entries
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.034
# In a cpio archive of a file, a hard link to it, a directory, a symbolic
# link and a FIFO, the fields hold the mode (type and permission bits), uid,
# gid and mtime of each file, the name size counts the NUL, c_nlink is at
# least the number of names in the archive, linked names share c_dev and
# c_ino while the others differ, directories and FIFOs have a zero file
# size, the symbolic link holds its target as data, entries follow each
# other without padding up to the trailer, and extraction restores the types
# and the hard link; a file with mode 6777 has those mode bits in c_mode.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.034
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
# a new file may take its directory's group: give m the effective group,
# so that its owner may set the set-group-ID bit without privilege
printf 'm\n' >m && chgrp "$(id -g)" m && chmod 6777 m|| pc_unresolved "setup: cannot make m with mode 6777"
[ "$(ls -l m | cut -c1-10)" = '-rwsrwsrwx' ] || pc_unresolved "setup: the set-id bits of m could not be set"
set -- $(ls -ln f1)
uid=$3; gid=$4
fld() { dd if="$1" bs=1 skip="$2" count="$3" 2>/dev/null | tr -d '\000'; }
num() { echo $((0$(fld "$1" "$2" "$3"))); }
ino() { ls -i "$1" | awk '{ print $1 }'; }
# cscan <archive>: cscan.out gets "name mode dev:ino nlink uid gid mtime namesize filesize offset"
cscan() {
	o=0; end=$(wc -c <"$1"); end=$((end + 0)); : >cscan.out; cerr=
	while [ "$o" -lt "$end" ]; do
		[ "$(fld "$1" "$o" 6)" = 070707 ] || { cerr="no magic 070707 at offset $o"; return; }
		ns=$(num "$1" $((o + 59)) 6); fs=$(num "$1" $((o + 65)) 11)
		[ "$ns" -ge 2 ] || { cerr="name size $ns at offset $o"; return; }
		nm=$(fld "$1" $((o + 76)) $((ns - 1)))
		echo "$nm $(num "$1" $((o + 18)) 6) $(fld "$1" $((o + 6)) 6):$(fld "$1" $((o + 12)) 6) $(num "$1" $((o + 36)) 6) $(num "$1" $((o + 24)) 6) $(num "$1" $((o + 30)) 6) $(num "$1" $((o + 48)) 11) $ns $fs $o" >>cscan.out
		[ ${#nm} -eq $((ns - 1)) ] || { cerr="the name of the entry at $o is not $((ns - 1)) characters"; return; }
		o=$((o + 76 + ns + fs))
		[ "$nm" = 'TRAILER!!!' ] && return
	done
	cerr="no trailer reached"
}
# e <name> <column>
e() { awk -v n="$1" -v c="$2" '$1 == n { print $c; exit }' cscan.out; }
pc_test
pax -w -x cpio f1 h d s p m >a 2>err </dev/null || pc_fail "pax -w -x cpio: nonzero status"
cscan a
[ -z "$cerr" ] || pc_fail "walking the entries by name and file sizes failed: $cerr"
[ "$(wc -l <cscan.out | tr -d ' ')" -eq 7 ] || pc_fail "the archive holds $(wc -l <cscan.out | tr -d ' ') entries, expected 6 files and the trailer"
for n in f1 h d s p m; do
	[ -n "$(e $n 1)" ] || pc_fail "$n is missing from the archive"
	[ "$(e $n 8)" -eq $((${#n} + 1)) ] || pc_fail "$n: c_namesize is $(e $n 8), expected $((${#n} + 1))"
done
[ "$(e 'TRAILER!!!' 9)" -eq 0 ] || pc_fail "the trailer has c_filesize $(e 'TRAILER!!!' 9), expected 0"
for n in f1 h; do
	[ "$(e $n 2)" -eq $((0100640)) ] || pc_fail "$n: c_mode is $(e $n 2) (decimal), expected 0100640"
	[ "$(e $n 5)" -eq "$uid" ] || pc_fail "$n: c_uid is $(e $n 5), expected $uid"
	[ "$(e $n 6)" -eq "$gid" ] || pc_fail "$n: c_gid is $(e $n 6), expected $gid"
	[ "$(e $n 7)" -eq 1041519845 ] || pc_fail "$n: c_mtime is $(e $n 7), expected 1041519845"
	[ "$(e $n 4)" -ge 2 ] || pc_fail "$n: c_nlink is $(e $n 4), expected at least 2 (two names in the archive)"
	fs=$(e $n 9)
	[ "$fs" -eq 0 ] || [ "$fs" -eq 6 ] || pc_fail "$n: c_filesize is $fs, expected 0 or 6"
done
[ "$(( $(e f1 9) + $(e h 9) ))" -ge 6 ] || pc_fail "neither f1 nor h carries the 6 octets of data"
[ "$(e f1 3)" = "$(e h 3)" ] || pc_fail "f1 and h are links but have c_dev:c_ino $(e f1 3) and $(e h 3)"
for n in d s p; do
	[ "$(e $n 4)" -ge 1 ] || pc_fail "$n: c_nlink is $(e $n 4), expected at least 1"
	[ "$(e $n 3)" = "$(e f1 3)" ] && pc_fail "$n has the c_dev:c_ino of f1 although it is not a link to it"
done
[ "$(e d 3)" != "$(e s 3)" ] && [ "$(e d 3)" != "$(e p 3)" ] && [ "$(e s 3)" != "$(e p 3)" ] || pc_fail "d, s and p do not have distinct c_dev:c_ino pairs"
[ $(( $(e d 2) & 0170000 )) -eq $((040000)) ] || pc_fail "d: the type bits of c_mode are not those of a directory"
[ $(( $(e d 2) & 0777 )) -eq $((0755)) ] || pc_fail "d: the permission bits of c_mode are not 755"
[ "$(e d 9)" -eq 0 ] || pc_fail "the directory has c_filesize $(e d 9), expected 0"
[ $(( $(e s 2) & 0170000 )) -eq $((0120000)) ] || pc_fail "s: the type bits of c_mode are not those of a symbolic link"
[ "$(e s 9)" -eq 2 ] || pc_fail "the symbolic link has c_filesize $(e s 9), expected 2 (the length of its target)"
[ "$(fld a $(($(e s 10) + 76 + $(e s 8))) 2)" = f1 ] || pc_fail "the data of the symbolic link is not its target f1"
[ $(( $(e m 2) & 07777 )) -eq $((06777)) ] || pc_fail "m: the permission and set-id bits of c_mode are not 6777"
[ $(( $(e m 2) & 0170000 )) -eq $((0100000)) ] || pc_fail "m: the type bits of c_mode are not those of a regular file"
[ $(( $(e p 2) & 0170000 )) -eq $((010000)) ] || pc_fail "p: the type bits of c_mode are not those of a FIFO"
[ "$(e p 9)" -eq 0 ] || pc_fail "the FIFO has c_filesize $(e p 9), expected 0"
rm -rf f1 h d s p m
pax -r <a >o 2>err || pc_fail "extracting the cpio archive: nonzero status"
[ "$(cat f1 2>/dev/null)" = 12345 ] || pc_fail "f1 was not restored"
[ -d d ] || pc_fail "the directory d was not restored"
[ -L s ] || pc_fail "the symbolic link s was not restored"
[ "$(ls -l s | sed 's/.* -> //')" = f1 ] || pc_fail "the symbolic link s was restored with another target"
[ -p p ] || pc_fail "the FIFO p was not restored"
[ -f h ] || pc_fail "the hard link h was not restored"
[ "$(ino f1)" = "$(ino h)" ] || pc_fail "h was restored as a separate file, not a hard link to f1"
pc_pass
