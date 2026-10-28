# cksum writes checksum, octet count and pathname for each file operand, and omits the pathname and its leading space for standard input.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cksum.003
. "$PC_CORE/pc.sh"
pc_begin XCU.cksum.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc' >f1 && printf '123456789' >f2 && : >f3 || pc_fail "cannot create input"
mkdir sub && printf 'abc' >sub/g || pc_unresolved "cannot create input"
pc_test
got=$(cksum f1 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1219131554 3 f1${NL}rc=0" ] || pc_fail "one file operand did not give '1219131554 3 f1'"
got=$(cksum f1 f2 f3 2>/dev/null </dev/null)
[ "$got" = "1219131554 3 f1${NL}930766865 9 f2${NL}4294967295 0 f3" ] || pc_fail "three file operands did not give three lines in order"
got=$(cksum <f1 2>/dev/null)
[ "$got" = "1219131554 3" ] || pc_fail "standard input did not give the checksum and count only"
got=$(cksum f2 f2 2>/dev/null </dev/null)
[ "$got" = "930766865 9 f2${NL}930766865 9 f2" ] || pc_fail "a repeated operand was not processed twice"
got=$(cksum sub/g 2>/dev/null </dev/null)
[ "$got" = "1219131554 3 sub/g" ] || pc_fail "the pathname was not written as given"
pc_pass
