# When there is no next line, N quits without copying the pattern space to the output, while n quits after the default output.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sed.014
. "$PC_CORE/pc.sh"
pc_begin XCU.sed.014
LC_ALL=POSIX
export LC_ALL
NL='
'
printf '1\n2\n3\n' >in || pc_fail "cannot create input"
pc_test
got=$(sed 'N;s/\n/+/' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1+2${NL}rc=0" ] || pc_fail "N on the last line did not quit without writing the pattern space"
got=$(sed 'n;d' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "1${NL}3${NL}rc=0" ] || pc_fail "n on the last line did not write the pattern space and quit"
pc_pass
