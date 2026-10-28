# cut -b selects bytes by single numbers and the three range forms.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cut.001
. "$PC_CORE/pc.sh"
pc_begin XCU.cut.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefgh\n' >in || pc_fail "cannot create input"
pc_test
got=$(cut -b 3 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "c${NL}rc=0" ] || pc_fail "-b 3 gave the wrong result"
got=$(cut -b 2-4 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "bcd${NL}rc=0" ] || pc_fail "-b 2-4 gave the wrong result"
got=$(cut -b -3 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abc${NL}rc=0" ] || pc_fail "-b -3 gave the wrong result"
got=$(cut -b 6- in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "fgh${NL}rc=0" ] || pc_fail "-b 6- gave the wrong result"
got=$(cut -b 1,3,5 in 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "ace${NL}rc=0" ] || pc_fail "-b 1,3,5 gave the wrong result"
pc_pass
