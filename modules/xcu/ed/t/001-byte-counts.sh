# ed byte counts and -s
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.001
# Loading a 24-byte file writes "24" to standard output and w writes "24"
# for the bytes written; with -s neither count is written. r of a file onto
# an empty buffer writes the byte count too.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
printf 'q\n' >s1; printf 'w g\nq\n' >s2; printf 'w h\nq\n' >s3; printf 'r f\nw k\nq\n' >s4
printf '24\n' >exp1; printf '24\n24\n' >exp2
pc_test
ed f <s1 >o1 2>err
cmp -s o1 exp1 || pc_fail "loading a 24-byte file wrote '$(cat o1)'"
ed -s f <s1 >o2 2>err
[ -s o2 ] && pc_fail "-s still wrote a byte count on load"
ed f <s2 >o3 2>err
cmp -s o3 exp2 || pc_fail "load and w wrote '$(cat o3)', expected 24 and 24"
cmp -s g f || pc_fail "w wrote a file that differs from the buffer"
ed -s f <s3 >o4 2>err
[ -s o4 ] && pc_fail "-s still wrote a byte count for w"
cmp -s h f || pc_fail "w under -s wrote a wrong file"
ed <s4 >o5 2>err
cmp -s o5 exp2 || pc_fail "r and w on an empty buffer wrote '$(cat o5)', expected 24 and 24"
cmp -s k f || pc_fail "r then w did not reproduce the file"
pc_pass
