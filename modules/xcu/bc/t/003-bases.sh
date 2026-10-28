# ibase and obase set the input and output radix, a single digit assigned to either is read as hexadecimal, and bases above 16 write space-led multi-digit decimal numbers.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bc.003
. "$PC_CORE/pc.sh"
pc_begin XCU.bc.003
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
b() { printf '%b\n' "$1" | bc 2>/dev/null; }
[ "$(b 'obase=16\n255')" = "FF" ] || pc_fail "255 in base 16 not FF"
[ "$(b 'obase=16\n10')" = "A" ] || pc_fail "10 in base 16 not A"
[ "$(b 'obase=2\n10')" = "1010" ] || pc_fail "10 in base 2 not 1010"
[ "$(b 'obase=8\n64')" = "100" ] || pc_fail "64 in base 8 not 100"
[ "$(b 'obase=16\n-255')" = "-FF" ] || pc_fail "-255 in base 16 not -FF"
[ "$(b 'obase=16\n0')" = "0" ] || pc_fail "zero in base 16 not 0"
[ "$(b 'ibase=2\n101')" = "5" ] || pc_fail "101 read in base 2 not 5"
[ "$(b 'ibase=16\nFF')" = "255" ] || pc_fail "FF read in base 16 not 255"
[ "$(b 'ibase=8\n17')" = "15" ] || pc_fail "17 read in base 8 not 15"
[ "$(b 'obase=16\nibase=2\n1111')" = "F" ] || pc_fail "1111 read in base 2 and written in base 16 not F"
[ "$(b 'ibase=2\nibase=A\n10')" = "10" ] || pc_fail "ibase=A did not set base ten whatever the current ibase"
[ "$(b 'ibase=16\nibase=A\n10')" = "10" ] || pc_fail "ibase=A from base 16 did not set base ten"
[ "$(b 'obase=A\n255')" = "255" ] || pc_fail "obase=A did not set base ten"
[ "$(b 'ibase')" = "10" ] || pc_fail "the initial ibase was not 10"
[ "$(b 'obase')" = "10" ] || pc_fail "the initial obase was not 10"
[ "$(b 'scale=1\nobase=16\n1.5')" = "1.8" ] || pc_fail "1.5 in base 16 not 1.8"
[ "$(b 'obase=25\n1024')" = " 01 15 24" ] || pc_fail "1024 in base 25 not  01 15 24"
[ "$(b 'obase=125\n1024')" = " 008 024" ] || pc_fail "1024 in base 125 not  008 024"
[ "$(b 'obase=2\n255')" = "11111111" ] || pc_fail "255 in base 2 wrong"
[ "$(b 'obase=16\n4095')" = "FFF" ] || pc_fail "4095 in base 16 wrong"
pc_pass
