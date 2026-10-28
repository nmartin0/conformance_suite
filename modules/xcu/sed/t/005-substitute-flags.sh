# The s flags: a number replaces only that occurrence, g replaces all, p writes on replacement, and w appends to a file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.005
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.005
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'aaa\nbbb\naxa\n' >in || pc_fail "cannot create input"
pc_test
[ "$(echo aaaa | sed 's/a/X/3' 2>/dev/null)" = "aaXa" ] || pc_fail "flag 3 did not replace only the third occurrence"
[ "$(echo aaaa | sed 's/a/X/g' 2>/dev/null)" = "XXXX" ] || pc_fail "flag g did not replace all occurrences"
[ "$(echo aaaa | sed 's/a/X/' 2>/dev/null)" = "Xaaa" ] || pc_fail "without flags more than the first occurrence was replaced"
[ "$(echo 'ab ab ab' | sed 's/ab/X/2' 2>/dev/null)" = "ab X ab" ] || pc_fail "flag 2 failed"
got=$(sed -n 's/a/Z/p' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "Zaa${NL}Zxa${NL}rc=0" ] || pc_fail "-n with the p flag wrote the wrong lines"
got=$(sed 's/b/Q/p' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "aaa${NL}Qbb${NL}Qbb${NL}axa${NL}rc=0" ] || pc_fail "p flag without -n did not write the changed line twice"
rm -f out
sed -n 's/a/Y/w out' in >/dev/null 2>&1 </dev/null
got=$(cat out 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "Yaa${NL}Yxa${NL}rc=0" ] || pc_fail "the w flag did not append the changed lines to the file"
rm -f out
echo x | sed 's/y/z/w out' >/dev/null 2>&1
[ -f out ] || pc_fail "the w file was not created before processing"
[ ! -s out ] || pc_fail "the w file received a line with no replacement"
pc_pass
