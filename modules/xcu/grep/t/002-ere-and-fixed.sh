# grep -E treats patterns as extended regular expressions and -F as fixed strings, several to a pattern list.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.grep.002
. "$PC_CORE/pc.sh"
pc_begin XCU.grep.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc\nabbc\na.c\ndef\n' >in || pc_fail "cannot create input"
pc_test
got=$(grep -E 'ab+c|^d' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}abbc${NL}def${NL}rc=0" ] || pc_fail "-E alternation with + gave the wrong lines"
got=$(grep -F 'a.c' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a.c${NL}rc=0" ] || pc_fail "-F did not treat the dot as a literal character"
got=$(grep -F "a.c${NL}def" in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a.c${NL}def${NL}rc=0" ] || pc_fail "-F with two newline-separated strings gave the wrong lines"
got=$(grep -E 'a|d' in 2>/dev/null </dev/null | while IFS= read -r l; do printf '[%s]' "$l"; done)
[ "$got" = "[abc][abbc][a.c][def]" ] || pc_fail "-E 'a|d' gave the wrong lines"
pc_pass
