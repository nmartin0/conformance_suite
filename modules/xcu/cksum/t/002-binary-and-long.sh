# cksum handles bytes of every value and lengths that need more than one octet to record.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.cksum.002
. "$PC_CORE/pc.sh"
pc_begin XCU.cksum.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '\000' >nul1 || pc_unresolved "cannot create input"
printf '\000\000\000\000' >nul4 || pc_unresolved "cannot create input"
: >all256 || pc_unresolved "cannot create input"
i=0
while [ $i -lt 256 ]; do
	printf "\\$(printf '%03o' $i)" >>all256 || pc_unresolved "cannot create input"
	i=$((i + 1))
done
: >a300 || pc_unresolved "cannot create input"
i=0
while [ $i -lt 30 ]; do
	printf 'aaaaaaaaaa' >>a300 || pc_unresolved "cannot create input"
	i=$((i + 1))
done
pc_test
got=$(cksum <nul1 2>/dev/null)
[ "$got" = "4215202376 1" ] || pc_fail "one NUL byte did not give 4215202376 1"
got=$(cksum <nul4 2>/dev/null)
[ "$got" = "3975907619 4" ] || pc_fail "four NUL bytes did not give 3975907619 4"
got=$(cksum <all256 2>/dev/null)
[ "$got" = "1313719201 256" ] || pc_fail "the 256 byte values did not give 1313719201 256"
got=$(cksum <a300 2>/dev/null)
[ "$got" = "1664553091 300" ] || pc_fail "300 letters a did not give 1664553091 300"
pc_pass
