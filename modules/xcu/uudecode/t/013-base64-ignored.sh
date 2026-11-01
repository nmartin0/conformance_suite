# uudecode ignores line breaks and other characters in Base64 data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uudecode.013
# MUTANT    uudecode.b64_quad_per_line
# MUTANT    uudecode.b64_blank_error
# MUTANT    uudecode.b64_space_error
# MUTANT    uudecode.status1
# MUTANT    uudecode.drop_last
# In Base64 data, line breaks and characters that are not in the alphabet
# are ignored when decoding (XCU uuencode, Base64 algorithm); a group of four
# characters may thus be split across lines or contain a blank.
. "$PC_CORE/pc.sh"
pc_begin XCU.uudecode.013
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'begin-base64 644 w.out\nQ2\nF0Q\n\n2F 0\n====\n' >in || pc_unresolved "setup: cannot write input"
printf CatCat >exp || pc_unresolved "setup: cannot write expectation"
pc_test
uudecode in >stdout 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "exit status $rc"
[ -f w.out ] || pc_fail "w.out was not created"
cmp -s exp w.out || pc_fail "decoded content is wrong"
pc_pass
