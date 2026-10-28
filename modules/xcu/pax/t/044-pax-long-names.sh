# pax -x pax: names and link targets longer than ustar allows
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.044
# The pax format archives a pathname of 304 characters and a symbolic link
# whose target has 150 characters, which ustar cannot hold, without error;
# the archive holds a path record and a linkpath record with them, and
# extraction restores the file and the link with the same names.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.044
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
rep() { awk -v c="$1" -v n="$2" 'BEGIN { s = ""; for (i = 0; i < n; i++) s = s c; printf "%s", s }'; }
D=$(rep d 60); long=$D/$D/$D/$D/$D
mkdir -p "$D/$D/$D/$D" || pc_unresolved "setup: cannot make a directory path of 243 characters"
printf 'LONGDATA\n' >"$long" || pc_unresolved "setup: cannot write the 304-character path"
tgt=$(rep x 150)
ln -s "$tgt" lnk || pc_unresolved "setup: cannot make the symbolic link"
pc_test
pax -w -x pax "$long" lnk >a 2>err </dev/null || pc_fail "pax -w -x pax of the long names: nonzero status"
tr -d '\000' <a | grep -F "path=$long" >/dev/null || pc_fail "the archive holds no path record with the 304-character pathname"
tr -d '\000' <a | grep -F "linkpath=$tgt" >/dev/null || pc_fail "the archive holds no linkpath record with the 150-character target"
got=$(pax <a 2>/dev/null | sort | tr '\n' ' ')
[ "$got" = "$long lnk " ] || pc_fail "the archive lists '$got'"
rm -rf "$D" lnk
pax -r <a >o 2>err || pc_fail "extracting the long names: nonzero status"
[ "$(cat "$long" 2>/dev/null)" = LONGDATA ] || pc_fail "the 304-character path was not restored with its contents"
[ "$(ls -l lnk 2>/dev/null | sed 's/.* -> //')" = "$tgt" ] || pc_fail "the symbolic link was not restored with its 150-character target"
pc_pass
