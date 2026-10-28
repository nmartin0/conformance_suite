# b and t branch to a label or the end of the script, { } groups commands, and ! negates the selection.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.011
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.011
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nx\nb\n' >in || pc_fail "cannot create input"
pc_test
got=$(sed -n -e '/x/b skip' -e p -e ':skip' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}rc=0" ] || pc_fail "b to a label did not skip the p"
got=$(sed -e '/x/b' -e 's/$/!/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a!${NL}x${NL}b!${NL}rc=0" ] || pc_fail "b with no label did not branch to the end of the script"
[ "$(echo aaaa | sed -e ':a' -e 's/aa/a/' -e 'ta' 2>/dev/null)" = "a" ] || pc_fail "t loop did not repeat while substitutions were made"
got=$(sed -e 's/a/A/' -e 't done' -e 's/$/-no/' -e ':done' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "A${NL}x-no${NL}b-no${NL}rc=0" ] || pc_fail "t did not branch only after a successful substitution"
got=$(sed -e 's/a/A/' -e 't' -e 's/$/-no/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "A${NL}x-no${NL}b-no${NL}rc=0" ] || pc_fail "t without a label failed"
got=$(sed -n '/x/!p' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}rc=0" ] || pc_fail "! gave the wrong lines"
got=$(sed -n '/x/ !p' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a${NL}b${NL}rc=0" ] || pc_fail "a blank before ! was not accepted"
got=$(sed -n '2,3{s/./Y/;p}' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "Y${NL}Y${NL}rc=0" ] || pc_fail "a braced group on a range gave the wrong result"
got=$(sed -n '1,2!{p}' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}rc=0" ] || pc_fail "! applied to a braced group gave the wrong result"
pc_pass
