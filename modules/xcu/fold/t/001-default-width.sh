# By default fold breaks lines that are longer than 80 columns after the 80th column.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fold.001
. "$PC_CORE/pc.sh"
pc_begin XCU.fold.001
LC_ALL=POSIX
export LC_ALL
NL='
'
ten=0123456789
w80=$ten$ten$ten$ten$ten$ten$ten$ten
printf '%s%s\n' "$w80" "$ten$ten" >long || pc_fail "cannot create input"
printf '%s\n' "$w80" >exact || pc_fail "cannot create input"
printf '%s\n' "$ten" >short || pc_fail "cannot create input"
pc_test
got=$(fold long 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${w80}${NL}${ten}${ten}${NL}rc=0" ] || pc_fail "a 100-column line was not broken after column 80"
got=$(fold exact 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${w80}${NL}rc=0" ] || pc_fail "an 80-column line was changed"
got=$(fold short 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "${ten}${NL}rc=0" ] || pc_fail "a short line was changed"
pc_pass
