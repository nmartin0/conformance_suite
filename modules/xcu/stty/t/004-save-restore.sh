# stty -g form is one line of safe characters and restores the settings
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.004
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.004
pc_pty_need
cat >inner.sh <<'IN'
stty -g >g.out 2>g.err || exit 1
g=`cat g.out`
stty -a >before.out || exit 2
stty -echo -icanon -isig -icrnl -opost || exit 3
stty -a >changed.out || exit 4
stty "$g" || exit 5
stty -a >after.out || exit 6
exit 0
IN
pc_test
pc_pty_run inner.sh
[ "$pc_pty_rc" -eq 0 ] || pc_fail "step $pc_pty_rc of the -g round trip failed"
[ "$(wc -l <g.out | tr -d ' ')" = 1 ] || pc_fail "-g did not write exactly one line"
g=$(cat g.out)
case $g in
*[!A-Za-z0-9._,:+=/@%^~#-]*) pc_fail "-g output holds a character outside the safe set" ;;
esac
[ -n "$g" ] || pc_fail "-g wrote an empty line"
cmp -s before.out changed.out && pc_fail "the changed settings were not visible in -a"
cmp -s before.out after.out || pc_fail "settings were not restored from the -g string ($(diff before.out after.out 2>&1 | tr '\n' '|' | cut -c1-300))"
pc_pass
