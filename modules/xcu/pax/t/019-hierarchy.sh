# pax: a directory brings in the file hierarchy rooted at it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.019
# "pax -w d h" and "pax -w" fed d on standard input archive d, d/e, d/e/f,
# d/g and h; listing with the pattern d (also with -n) names d and
# everything below it; "pax -r d" restores the hierarchy and not h;
# "pax -rw d dest" copies the nested files.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir d d/e || pc_unresolved "setup: mkdir failed"
printf 'a\n' >d/e/f && printf 'b\n' >d/g && printf 'c\n' >h || pc_unresolved "setup: cannot write files"
want4='d
d/e
d/e/f
d/g'
want5="$want4
h"
pc_test
pax -w d h >a1 2>err </dev/null || pc_fail "pax -w d h: nonzero status"
got=$(pax <a1 2>err | sort)
[ "$got" = "$want5" ] || pc_fail "the archive lists '$got', expected d, d/e, d/e/f, d/g and h"
printf 'd\n' | pax -w >a2 2>err || pc_fail "pax -w with d on standard input: nonzero status"
got=$(pax <a2 2>err | sort)
[ "$got" = "$want4" ] || pc_fail "the archive written from a name on standard input lists '$got'"
got=$(pax d <a1 2>err | sort)
[ "$got" = "$want4" ] || pc_fail "listing with the pattern d gave '$got'"
got=$(pax -n d <a1 2>err | sort)
[ "$got" = "$want4" ] || pc_fail "listing with -n d gave '$got', the directory still matches its hierarchy"
rm -rf d h
pax -r d <a1 >o 2>err || pc_fail "pax -r d: nonzero status"
[ "$(cat d/e/f 2>/dev/null)" = a ] || pc_fail "d/e/f was not extracted by the pattern d"
[ "$(cat d/g 2>/dev/null)" = b ] || pc_fail "d/g was not extracted by the pattern d"
[ -e h ] && pc_fail "h was extracted although only d was selected"
mkdir dest || pc_unresolved "setup: mkdir dest failed"
pax -rw d dest >o 2>err </dev/null || pc_fail "pax -rw d dest: nonzero status"
[ "$(cat dest/d/e/f 2>/dev/null)" = a ] || pc_fail "dest/d/e/f was not copied"
[ "$(cat dest/d/g 2>/dev/null)" = b ] || pc_fail "dest/d/g was not copied"
pc_pass
