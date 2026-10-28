# q, Q and the changed-buffer warning
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.013
# q on a modified buffer writes "?" and does not quit; a second q (after
# only non-modifying commands) quits; q on an unmodified buffer or after an
# empty a does not warn; Q never warns; nothing is written to the file
# without w.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat f >f.keep
printf '1d\nq\n1p\nq\n' >sa
printf 'q\n' >sb
printf 'a\n.\nq\n' >sc
printf 'a\nx\n.\nq\nQ\n' >sd
printf '1d\nQ\n' >se
printf '24\n?\ntwo\n' >expa
printf '?\n' >expd
pc_test
ed f <sa >oa 2>err
rc=$?
cmp -s oa expa || pc_fail "q on a changed buffer wrote '$(cat oa)', expected 24, ? and two"
cmp -s f f.keep || pc_fail "the file changed without a w command"
ed -s f <sb >ob 2>err
[ -s ob ] && pc_fail "q on an unmodified buffer wrote output"
ed -s f <sc >oc 2>err
[ -s oc ] && pc_fail "an empty a made q warn"
ed -s f <sd >od 2>err
cmp -s od expd || pc_fail "q after a real a wrote '$(cat od)', expected ?"
ed -s f <se >oe 2>err
[ -s oe ] && pc_fail "Q wrote output"
cmp -s f f.keep || pc_fail "the file changed without a w command"
pc_pass
