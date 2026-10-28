# With -E sed reads regular expressions as extended ones.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.007
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.007
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'ab\naab\nb\n' >in || pc_fail "cannot create input"
pc_test
got=$(sed -E -n '/^a+b$/p' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab${NL}aab${NL}rc=0" ] || pc_fail "-E with + gave the wrong lines"
[ "$(echo abcabc | sed -E 's/(abc){2}/X/' 2>/dev/null)" = "X" ] || pc_fail "-E interval on a group failed"
[ "$(echo 'ab' | sed -E 's/(a)(b)/\2\1/' 2>/dev/null)" = "ba" ] || pc_fail "-E back-references in the replacement failed"
[ "$(echo 'a|b' | sed 's/a|b/X/' 2>/dev/null)" = "X" ] || pc_fail "without -E, | was not an ordinary character"
[ "$(echo 'cat' | sed -E 's/cat|dog/pet/' 2>/dev/null)" = "pet" ] || pc_fail "-E alternation failed"
[ "$(echo 'ab' | sed -nE 's/b/B/p' 2>/dev/null)" = "aB" ] || pc_fail "-nE combined options failed"
pc_pass
