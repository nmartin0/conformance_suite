# An empty RE reuses the last RE used, several files are edited as one concatenation, and q ends processing.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.013
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.013
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc\nxbz\n' >f1 && printf 'ab\n' >f2 || pc_fail "cannot create input"
pc_test
[ "$(echo abc | sed '/b/s//X/' 2>/dev/null)" = "aXc" ] || pc_fail "an empty RE did not reuse the address RE"
[ "$(echo abcabc | sed 's/b/X/;s//Y/' 2>/dev/null)" = "aXcaYc" ] || pc_fail "an empty RE did not reuse the last s RE"
got=$(sed 's/b/B/' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "aBc${NL}xBz${NL}aB${NL}rc=0" ] || pc_fail "two files were not edited in order"
got=$(sed -n '$p' f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}rc=0" ] || pc_fail "\$ did not mean the last line of the last file"
got=$(sed 1q f1 f2 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}rc=0" ] || pc_fail "q did not end processing after line 1"
got=$(sed -n '1{p;q}' f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}rc=0" ] || pc_fail "p;q in braces failed"
got=$(sed '1{a\
Q
q
}' f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}Q${NL}rc=0" ] || pc_fail "q did not write queued text before quitting"
pc_pass
