# The control characters that the <control>-X notation names are the ones
# the utilities know by those names: od -t a names byte values 1 to 31 and
# 127 (SOH, STX, ... US, DEL) in the order of the table.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.10.2-output-devices-and-terminal-types.001
. "$PC_CORE/pc.sh"
pc_begin XBD.10.2-output-devices-and-terminal-types.001
LC_ALL=C; export LC_ALL
pc_test
# value (octal) and the name of <control>-X for X = A..Z [ \ ] ^ _ ?
set -- 001 soh 002 stx 003 etx 004 eot 005 enq 006 ack 007 bel 010 bs 011 ht 012 lf 013 vt 014 ff 015 cr 016 so 017 si \
    020 dle 021 dc1 022 dc2 023 dc3 024 dc4 025 nak 026 syn 027 etb 030 can 031 em 032 sub 033 esc 034 fs 035 gs 036 rs 037 us 177 del
while [ $# -ge 2 ]; do
	oct=$1
	want=$2
	shift 2
	got=$(printf "\\$oct" | od -An -t a 2>/dev/null | tr -d ' \t\n')
	if [ "$oct" = 012 ]; then
		case $got in lf | nl) ;; *) pc_fail "byte \\$oct is shown by od -t a as '$got', expected lf or nl" ;; esac
	else
		[ "$got" = "$want" ] || pc_fail "byte \\$oct is shown by od -t a as '$got', expected $want"
	fi
done
pc_pass
