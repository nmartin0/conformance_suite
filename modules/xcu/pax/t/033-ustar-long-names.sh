# pax -x ustar: name and prefix, and what does not fit
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.033
# A ustar pathname of 152 characters is stored with name and prefix fields
# that join into it and is listed and extracted whole; a pathname of 304
# characters, or a symbolic link whose target is 150 characters, is refused
# with a diagnostic and a nonzero status and no part of it is stored, while
# other files are archived.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.033
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
rep() { awk -v c="$1" -v n="$2" 'BEGIN { s = ""; for (i = 0; i < n; i++) s = s c; printf "%s", s }'; }
fld() { dd if="$1" bs=1 skip="$2" count="$3" 2>/dev/null | tr -d '\000'; }
A=$(rep a 60); B=$(rep b 60); C=$(rep c 30)
mid=$A/$B/$C
mkdir -p "$A/$B" || pc_unresolved "setup: cannot make a directory path of 121 characters"
printf 'MIDDATA\n' >"$mid" || pc_unresolved "setup: cannot write the 152-character path"
printf 'small\n' >s1 || pc_unresolved "setup: cannot write s1"
D=$(rep d 60); long=$D/$D/$D/$D/$D
mkdir -p "$D/$D/$D/$D" || pc_unresolved "setup: cannot make a directory path of 243 characters"
printf 'LONGDATA\n' >"$long" || pc_unresolved "setup: cannot write the 304-character path"
tgt=$(rep x 150)
ln -s "$tgt" lnk || pc_unresolved "setup: cannot make the symbolic link"
pc_test
pax -w -x ustar "$mid" >a1 2>err </dev/null || pc_fail "pax -w -x ustar of a 152-character path: nonzero status"
nm=$(fld a1 0 100); pre=$(fld a1 345 155)
if [ -n "$pre" ]; then full=$pre/$nm; else full=$nm; fi
[ "$full" = "$mid" ] || pc_fail "prefix '$pre' and name '$nm' do not join into the pathname"
[ "$(pax <a1 2>/dev/null)" = "$mid" ] || pc_fail "the archive does not list the 152-character path"
rm -rf "$A"
pax -r <a1 >o 2>err || pc_fail "extracting the 152-character path: nonzero status"
[ "$(cat "$mid" 2>/dev/null)" = MIDDATA ] || pc_fail "the 152-character path was not restored with its contents"
pax -w -x ustar s1 "$long" >a2 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a 304-character pathname gave exit status 0"
[ -s err ] || pc_fail "no diagnostic for the 304-character pathname"
got=$(pax <a2 2>/dev/null)
[ "$got" = s1 ] || pc_fail "after the refusal the archive lists '$got', expected s1 alone"
grep LONGDATA a2 >/dev/null && pc_fail "the data of the refused file was stored"
pax -w -x ustar s1 lnk >a3 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "a link target of 150 characters gave exit status 0"
[ -s err ] || pc_fail "no diagnostic for the link target of 150 characters"
got=$(pax <a3 2>/dev/null)
[ "$got" = s1 ] || pc_fail "after the refusal the archive lists '$got', expected s1 alone"
pc_pass
