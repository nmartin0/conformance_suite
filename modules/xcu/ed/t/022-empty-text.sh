# a and i with empty text
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.022
# a and i whose text is empty leave the buffer change flag alone, so q does
# not warn; the line holding the lone dot is not part of the text.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.022
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf 'i\n.\nq\n' >s1
printf '$a\n.\nq\n' >s2
printf '$a\nx\n.\n1i\ny\n.\n,p\nQ\n' >s3
printf 'y\none\ntwo\nthree\nfour\nfive\nx\n' >exp3
pc_test
ed -s f <s1 >o1 2>err
rc=$?
[ "$rc" -eq 0 ] && [ ! -s o1 ] || pc_fail "q after an empty i: status $rc, output '$(cat o1)'"
ed -s f <s2 >o2 2>err
rc=$?
[ "$rc" -eq 0 ] && [ ! -s o2 ] || pc_fail "q after an empty a: status $rc, output '$(cat o2)'"
ed -s f <s3 >o3 2>err
cmp -s o3 exp3 || pc_fail "a and i text gave '$(tr '\n' ' ' <o3)'"
pc_pass
