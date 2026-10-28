# tr -c and -C use the complement of string1; -cd deletes everything but string1; -cs squeezes the replacement.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.tr.004
. "$PC_CORE/pc.sh"
pc_begin XCU.tr.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'ab1 2c\n' >in1 || pc_fail "cannot create input"
printf 'a1b22\n' >in2 || pc_fail "cannot create input"
printf 'ab, cd.ef\n' >in3 || pc_fail "cannot create input"
pc_test
got=$(tr -c 'a-z\n' '[X*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abXXXc${NL}rc=0" ] || pc_fail "tr -c 'a-z\\n' '[X*]' gave the wrong result"
got=$(tr -C 'a-z\n' '[X*]' <in1 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "abXXXc${NL}rc=0" ] || pc_fail "tr -C 'a-z\\n' '[X*]' gave the wrong result"
got=$(tr -cd '0-9\n' <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "122${NL}rc=0" ] || pc_fail "tr -cd '0-9\\n' gave the wrong result"
got=$(tr -Cd '0-9\n' <in2 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "122${NL}rc=0" ] || pc_fail "tr -Cd '0-9\\n' gave the wrong result"
got=$(tr -cs 'a-z' '[_*]' <in3 2>/dev/null; printf 'rc=%s' "$?")
[ "$got" = "ab_cd_ef_rc=0" ] || pc_fail "tr -cs 'a-z' '[_*]' gave the wrong result"
pc_pass
