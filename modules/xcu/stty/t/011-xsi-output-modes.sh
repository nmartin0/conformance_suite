# XSI output modes and delay styles
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.011
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.011
pc_pty_need
cat >inner.sh <<'IN'
snap() { stty -a | tr ';' ' ' >"$1"; }
for m in onlcr ocrnl onocr onlret ofill ofdel; do
	for sign in "" -; do
		stty "$sign$m" 2>/dev/null; echo "$m $sign $?" >>rc.out; snap "o.$m.$sign.out"
	done
done
for d in cr0 cr1 cr2 cr3 nl0 nl1 tab0 tab1 tab2 tab3 bs0 bs1 ff0 ff1 vt0 vt1; do
	stty "$d" 2>/dev/null; echo "$d $?" >>rc.out; snap "d.$d.out"
done
stty tab3; stty tabs; echo "tabs $?" >>rc.out; snap tabs.out
stty -tabs; echo "-tabs $?" >>rc.out; snap notabs.out
IN
pc_test
pc_pty_run inner.sh
has() { for w in $(cat "$1"); do [ "$w" = "$2" ] && return 0; done; return 1; }
ok() { grep "^$1 0$" rc.out >/dev/null; }
for m in onlcr ocrnl onocr onlret ofill ofdel; do
	ok "$m " || pc_fail "stty $m failed"
	has "o.$m..out" "$m" || pc_fail "after stty $m the output lacks $m"
	ok "$m -" || pc_fail "stty -$m failed"
	has "o.$m.-.out" "-$m" || pc_fail "after stty -$m the output lacks -$m"
done
for d in cr0 cr1 cr2 cr3 nl0 nl1 tab0 tab1 tab2 tab3 bs0 bs1 ff0 ff1 vt0 vt1; do
	if ok "$d"; then has "d.$d.out" "$d" || pc_fail "after stty $d the output lacks $d"; fi
done
ok tabs || pc_fail "stty tabs failed"
has tabs.out tab0 || pc_fail "stty tabs is not tab0"
ok -tabs || pc_fail "stty -tabs failed"
has notabs.out tab3 || pc_fail "stty -tabs is not tab3"
pc_pass
