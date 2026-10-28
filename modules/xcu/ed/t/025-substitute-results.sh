# s failure, success and current line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.025
# s is an error only when it fails on every addressed line; a replacement
# equal to the text still counts as a substitution; the current line is the
# last line substituted; a lone % with no earlier s is an error and % inside a longer
# replacement or escaped is literal.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.025
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
# the buffers are changed, so scripts that expect exact output end with Q (end of input acts as q, which warns on a changed buffer)
printf '1s,one,uno,\n2s#t\\(w\\)o#[\\1]&#\n3s/e/E/2\n4s/o/0/g\n1,5s/e/E/\n4s/u/u/\n,p\nQ\n' >s1
printf 'uno\n[w]two\nthrEE\nf0ur\nfivE\n' >exp1
printf '1,5s/o/0/\n.=\nQ\n' >s2
printf '4\n' >exp2
printf '1s/one/X/\n2s/two/%%/\n3s/three/%%%%/\n4s/four/\\%%/\n,p\nQ\n' >s3
printf 'X\nX\n%%%%\n%%\nfive\n' >exp3
printf '1s/one/%%/\n' >s4
printf '5s/zzz/x/\n' >s5
printf '2,3s/zzz/q/\n' >s6
pc_test
ed -s f <s1 >o1 2>err
cmp -s o1 exp1 || pc_fail "s results were '$(tr '\n' ' ' <o1)', expected '$(tr '\n' ' ' <exp1)'"
ed -s f <s2 >o2 2>err
cmp -s o2 exp2 || pc_fail "current line after s was '$(cat o2)', expected 4"
ed -s f <s3 >o3 2>err
cmp -s o3 exp3 || pc_fail "% handling gave '$(tr '\n' ' ' <o3)', expected '$(tr '\n' ' ' <exp3)'"
ed -s f <s4 >o4 2>err
[ "$(cat o4)" = '?' ] || pc_fail "a lone % with no earlier s gave '$(cat o4)', expected ?"
for e in s5 s6; do
	ed -s f <$e >o.$e 2>err
	[ "$(cat o.$e)" = '?' ] || pc_fail "a substitution failing on every addressed line ($e) gave '$(cat o.$e)', expected ?"
done
pc_pass
