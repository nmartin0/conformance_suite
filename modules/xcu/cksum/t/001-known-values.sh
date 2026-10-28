# cksum writes the CRC and octet count defined by the page for fixed inputs.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cksum.001
. "$PC_CORE/pc.sh"
pc_begin XCU.cksum.001
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
c() { printf "$1" | cksum 2>/dev/null; }
[ "$(c 'abc')" = "1219131554 3" ] || pc_fail "abc did not give 1219131554 3"
[ "$(c 'a')" = "1220704766 1" ] || pc_fail "a did not give 1220704766 1"
[ "$(c '123456789')" = "930766865 9" ] || pc_fail "123456789 did not give 930766865 9"
[ "$(c 'hello\n')" = "3015617425 6" ] || pc_fail "hello and a newline did not give 3015617425 6"
[ "$(c 'abc\n')" = "1112837078 4" ] || pc_fail "abc and a newline did not give 1112837078 4"
[ "$(c '')" = "4294967295 0" ] || pc_fail "empty input did not give 4294967295 0"
pc_pass
