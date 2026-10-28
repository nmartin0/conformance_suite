# sleep 2 lasts at least two seconds.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sleep.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sleep.002
# secs: whole seconds into the day, from the clock
secs() {
	IFS=: read -r h m s <<PCX
$(date -u +%H:%M:%S)
PCX
	h=${h#0}
	m=${m#0}
	s=${s#0}
	echo $((h * 3600 + m * 60 + s))
}
t0=$(secs)
case $t0 in
''|*[!0-9]*) pc_unresolved "setup: cannot read the clock with date -u +%H:%M:%S" ;;
esac
pc_test
sleep 2 </dev/null >/dev/null 2>&1
rc=$?
t1=$(secs)
case $t1 in
''|*[!0-9]*) pc_unresolved "cannot read the clock after sleeping" ;;
esac
[ "$rc" -eq 0 ] || pc_fail "sleep 2 exited $rc"
d=$((t1 - t0))
[ "$d" -ge 0 ] || d=$((d + 86400))
[ "$d" -ge 2 ] || pc_fail "the clock moved only $d seconds during sleep 2"
pc_pass
