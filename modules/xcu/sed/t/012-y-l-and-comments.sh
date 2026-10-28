# y translates characters, l writes an unambiguous form ending in $ with escapes and octal, and # starts a comment, #n on the first line meaning -n.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.012
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.012
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n' >in || pc_fail "cannot create input"
pc_test
[ "$(echo hello | sed 'y/el/ip/' 2>/dev/null)" = "hippo" ] || pc_fail "y did not translate"
[ "$(echo 'a/b' | sed 'y,/,|,' 2>/dev/null)" = "a|b" ] || pc_fail "y with another delimiter failed"
[ "$(echo 'a/b' | sed 'y/\//|/' 2>/dev/null)" = "a|b" ] || pc_fail "y with an escaped delimiter failed"
got=$(printf 'a\nb\n' | sed 'N;y/\n/,/' 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "a,b${NL}rc=0" ] || pc_fail "y with \\n failed"
[ "$(printf '%s\n' 'a\b' | sed 'y/\\/X/' 2>/dev/null)" = "aXb" ] || pc_fail "y with an escaped backslash failed"
got=$(printf 'a\tb\\c\001d\n' | sed -n l 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = 'a\tb\\c\001d$'"${NL}rc=0" ] || pc_fail "l did not give a\\tb\\\\c\\001d\$"
got=$(printf 'x\n' | sed -n l 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "x\$${NL}rc=0" ] || pc_fail "l did not end the line with \$"
got=$(printf 'a\bb\fc\rd\ve\n' | sed -n l 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = 'a\bb\fc\rd\ve$'"${NL}rc=0" ] || pc_fail "l did not write \\b \\f \\r \\v as escapes"
got=$(sed '# a comment
s/1/one/;# another
' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "one${NL}2${NL}rc=0" ] || pc_fail "comments were not ignored"
got=$(sed '#n
2p' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "2${NL}rc=0" ] || pc_fail "#n on the first line did not suppress default output"
got=$(sed -e '1d' -e '#n' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "2${NL}rc=0" ] || pc_fail "#n not on the first line suppressed output"
pc_pass
