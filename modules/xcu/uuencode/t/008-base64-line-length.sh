# uuencode -m lines of at most 76 characters
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uuencode.008
# With -m, 100 input bytes encode to the expected 136-character string (33
# groups and a padded final group) once the line breaks are removed, and no
# line is longer than 76 characters.
. "$PC_CORE/pc.sh"
pc_begin XCU.uuencode.008
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '%0100d' 0 >in || pc_unresolved "setup: cannot write input"
sz=$(wc -c <in); sz=$((sz + 0))
[ "$sz" -eq 100 ] || pc_unresolved "setup: input is $sz bytes"
exp=
i=0
while [ "$i" -lt 33 ]; do
	exp=${exp}MDAw
	i=$((i + 1))
done
exp=${exp}MA==
pc_test
uuencode -m dest.bin <in >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
l1=$(sed -n 1p out)
case $l1 in
begin-base64\ *\ dest.bin) ;;
*) pc_fail "begin line has the wrong shape: $l1" ;;
esac
last=$(sed -n '$p' out)
[ "$last" = '====' ] || pc_fail "last line is '$last', expected '===='"
sed '1d;$d' out >data
while IFS= read -r l; do
	[ "${#l}" -le 76 ] || pc_fail "an encoded line has ${#l} characters"
done <data
got=$(tr -d '\n' <data)
[ "$got" = "$exp" ] || pc_fail "encoded data differs from the expected Base64 text"
pc_pass
