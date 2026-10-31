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
# every character is printable and none is white space or one of * ? [
case $g in
*[![:graph:]]*) pc_fail "-g output holds a character that is not printable, or white space" ;;
*['*?[']*) pc_fail "-g output holds a pathname-expansion character" ;;
esac
[ -n "$g" ] || pc_fail "-g wrote an empty line"
cmp -s before.out changed.out && pc_fail "the changed settings were not visible in -a"
# The page asks for the settings to be applied again, not for every item of the
# report (which can hold items of its own) to come back, so compare the
# settings that the test changed and the control characters.
tok() { tr ';\n' '  ' <"$1" | tr -s ' \t' '\n\n'; }
flag() { tok "$1" | grep -x -e "$2" -e "-$2" | sed -n 1p; }
ctl() { tok "$1" | awk -v n="$2" '$0 == n { getline; if ($0 == "=") { getline; print; exit } }'; }
for f in echo icanon isig icrnl opost; do
	[ -n "$(flag before.out $f)" ] || pc_fail "the report does not show $f"
	[ "$(flag before.out $f)" = "$(flag after.out $f)" ] || pc_fail "$f was not restored from the -g string: $(flag before.out $f) became $(flag after.out $f)"
done
for c in erase kill intr eof; do
	[ "$(ctl before.out $c)" = "$(ctl after.out $c)" ] || pc_fail "the $c character was not restored from the -g string"
done
pc_pass
