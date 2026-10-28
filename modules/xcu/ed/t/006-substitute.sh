# the s command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.006
# s replaces the first match, all with g, only the nth with a count; & is
# the matched text, \1 \2 are back-references, a lone % repeats the previous
# replacement, and another delimiter character may be used.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'aaa bbb\nfoo foo foo\nx.y\nhello\nzz zz\na/b\n' >f || pc_unresolved "setup: cannot write f"
cat >s <<'EOS'
1s/a/X/
2s/foo/bar/g
3s/\./,/
4s/l/L/2
4s/\(h\)\(e\)/\2\1&/
5s/z/Y/
5s/z/%/
6s#/#-#
,p
Q
EOS
printf 'Xaa bbb\nbar bar bar\nx,y\nehhelLo\nYY zz\na-b\n' >exp
pc_test
ed -s f <s >o 2>err
cmp -s o exp || pc_fail "substitutions gave: $(cat o)"
pc_pass
