# pax: reading ustar headers whose name or prefix field is unusual
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.046
# Reading a ustar archive: a prefix field that begins with NUL is ignored
# (the name alone is the pathname), and a name field of 100 characters with
# no terminating NUL is accepted; a 100-character file name is written and
# read back whole.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.046
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
rep() { awk -v c="$1" -v n="$2" 'BEGIN { s = ""; for (i = 0; i < n; i++) s = s c; printf "%s", s }'; }
# patch <file> <offset> <printf format>: overwrite octets in place
patch() { printf "$3" | dd of="$1" bs=1 seek="$2" conv=notrunc 2>/dev/null; }
# fixsum <file>: recompute the checksum of the first header
fixsum() {
	s=$(od -An -v -tu1 -N512 "$1" | awk '{ for (i = 1; i <= NF; i++) { n++; if (n >= 149 && n <= 156) t += 32; else t += $i } } END { print t }')
	patch "$1" 148 "$(printf '%06o' "$s")\\000 "
}
printf 'data\n' >f1 || pc_unresolved "setup: cannot write f1"
pax -w -x ustar f1 >base 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
N100=$(rep n 100)
printf 'wide\n' >"$N100" || pc_unresolved "setup: cannot write a file with a 100-character name"
pc_test
cp base a1 || pc_unresolved "setup: cp failed"
patch a1 346 'zzz'
fixsum a1
[ "$(pax <a1 2>err)" = f1 ] || pc_fail "a prefix beginning with NUL was not ignored: the archive lists '$(pax <a1 2>/dev/null)'"
rm -f f1
pax -r <a1 >o 2>err || pc_fail "extracting with a prefix that begins with NUL: nonzero status"
[ "$(cat f1 2>/dev/null)" = data ] || pc_fail "f1 was not restored from the archive whose prefix begins with NUL"
cp base a2 || pc_unresolved "setup: cp failed"
patch a2 0 "$N100"
fixsum a2
[ "$(pax <a2 2>err)" = "$N100" ] || pc_fail "a name field of 100 characters without a NUL is read as '$(pax <a2 2>/dev/null)'"
pax -w -x ustar "$N100" >a3 2>err </dev/null || pc_fail "pax -w -x ustar of a 100-character name: nonzero status"
[ "$(pax <a3 2>/dev/null)" = "$N100" ] || pc_fail "the 100-character name is listed as '$(pax <a3 2>/dev/null)'"
rm -f "$N100"
pax -r <a3 >o 2>err || pc_fail "extracting the 100-character name: nonzero status"
[ "$(cat "$N100" 2>/dev/null)" = wide ] || pc_fail "the file with the 100-character name was not restored"
pc_pass
