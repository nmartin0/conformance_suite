# pax -s: &, back-references, g, delimiters, empty results, symbolic links
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.025
# In -s, & stands for the matched text and \1 \2 for subexpressions, the g
# flag replaces every match, another delimiter such as # can be used, a name
# that becomes empty is ignored when writing and when reading, the
# substitution is applied to every member of a hierarchy, and the s and S
# flags decide whether the contents of a symbolic link are substituted (S:
# yes, s: no) in write and read mode.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.025
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '1\n' >f1 && printf '2\n' >f2 && printf '3\n' >a.a.a || pc_unresolved "setup: cannot write files"
mkdir d && printf 'x\n' >d/x || pc_unresolved "setup: cannot make d"
ln -s tgt l || pc_unresolved "setup: ln -s failed"
names() { pax <"$1" 2>/dev/null | sort | tr '\n' ' '; }
# check <expected> <message> <archive>
check() { got=$(names "$3"); [ "$got" = "$1" ] || pc_fail "$2: archive lists '$got', expected '$1'"; }
pc_test
pax -w -s ',f,&&,' f1 >a1 2>err </dev/null || pc_fail "-s with &: nonzero status"
check 'ff1 ' "-s ',f,&&,'" a1
pax -w -s ',\(.\)\(.\),\2\1,' f1 >a2 2>err </dev/null || pc_fail "-s with back-references: nonzero status"
check '1f ' "-s with \\2\\1" a2
pax -w -s ',a,b,g' a.a.a >a3 2>err </dev/null || pc_fail "-s with g: nonzero status"
check 'b.b.b ' "-s ',a,b,g'" a3
pax -w -s ',a,b,' a.a.a >a4 2>err </dev/null || pc_fail "-s without g: nonzero status"
check 'b.a.a ' "-s ',a,b,'" a4
pax -w -s '#f#g#' f1 >a5 2>err </dev/null || pc_fail "-s with delimiter #: nonzero status"
check 'g1 ' "-s '#f#g#'" a5
pax -w -s ',^f1$,,' f1 f2 >a6 2>/dev/null </dev/null
check 'f2 ' "writing with a name that becomes empty" a6
pax -w f1 f2 >a7 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
rm -f f1 f2
pax -r -s ',^f1$,,' <a7 >o 2>/dev/null
[ -e f1 ] && pc_fail "reading: f1 was extracted although its name became empty"
[ "$(cat f2 2>/dev/null)" = 2 ] || pc_fail "reading: f2 was not extracted"
pax -w -s ',^d,e,' d >a8 2>err </dev/null || pc_fail "-s on a hierarchy: nonzero status"
check 'e e/x ' "-s ',^d,e,' on d" a8
fld() { dd if="$1" bs=1 skip=157 count=100 2>/dev/null | tr -d '\000'; }
pax -w -x ustar -s ',tgt,new,S' l >a9 2>err </dev/null || pc_fail "-s with S: nonzero status"
[ "$(fld a9)" = new ] || pc_fail "with the S flag the stored link contents are '$(fld a9)', expected new"
pax -w -x ustar -s ',tgt,new,s' l >a10 2>err </dev/null || pc_fail "-s with s: nonzero status"
[ "$(fld a10)" = tgt ] || pc_fail "with the s flag the stored link contents are '$(fld a10)', expected tgt"
pax -w -x ustar l >a11 2>err </dev/null || pc_unresolved "setup: pax -w of the link failed"
rm -f l
pax -r -s ',tgt,new,S' <a11 >o 2>err || pc_fail "reading with the S flag: nonzero status"
got=$(ls -l l 2>/dev/null | sed 's/.* -> //')
[ "$got" = new ] || pc_fail "reading with the S flag: the link points to '$got', expected new"
rm -f l
pax -r -s ',tgt,new,s' <a11 >o 2>err || pc_fail "reading with the s flag: nonzero status"
got=$(ls -l l 2>/dev/null | sed 's/.* -> //')
[ "$got" = tgt ] || pc_fail "reading with the s flag: the link points to '$got', expected tgt"
pc_pass
