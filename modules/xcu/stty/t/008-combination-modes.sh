# combination modes: evenp, oddp, parity, nl, ek, sane
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.stty.008
#
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.stty.008
pc_pty_need
cat >inner.sh <<'IN'
snap() { stty -a | tr ';' ' ' >"$1"; }
stty evenp 2>/dev/null; echo "evenp $?" >>rc.out; snap evenp.out
stty parity 2>/dev/null; echo "parity $?" >>rc.out; snap parity.out
stty oddp 2>/dev/null; echo "oddp $?" >>rc.out; snap oddp.out
stty -parity 2>/dev/null; echo "-parity $?" >>rc.out; snap nopar.out
stty evenp 2>/dev/null; stty -evenp 2>/dev/null; echo "-evenp $?" >>rc.out; snap noeven.out
stty oddp 2>/dev/null; stty -oddp 2>/dev/null; echo "-oddp $?" >>rc.out; snap noodd.out
stty nl; echo "nl $?" >>rc.out; snap nl.out
stty -nl; echo "-nl $?" >>rc.out; snap nonl.out
stty ek; echo "ek $?" >>rc.out
stty sane; echo "sane $?" >>rc.out
IN
pc_test
pc_pty_run inner.sh
has() { for w in $(cat "$1"); do [ "$w" = "$2" ] && return 0; done; return 1; }
ok() { grep "^$1 0$" rc.out >/dev/null; }
if ok evenp; then has evenp.out parenb && has evenp.out cs7 && has evenp.out -parodd || pc_fail "evenp did not give parenb cs7 -parodd"; fi
if ok parity; then has parity.out parenb && has parity.out cs7 && has parity.out -parodd || pc_fail "parity did not give parenb cs7 -parodd"; fi
if ok oddp; then has oddp.out parenb && has oddp.out cs7 && has oddp.out parodd || pc_fail "oddp did not give parenb cs7 parodd"; fi
for f in nopar noeven noodd; do
	n=-parity; [ $f = noeven ] && n=-evenp; [ $f = noodd ] && n=-oddp
	if ok "$n"; then has $f.out -parenb && has $f.out cs8 || pc_fail "$n did not give -parenb cs8"; fi
done
ok nl || pc_fail "stty nl failed"
has nl.out -icrnl || pc_fail "nl did not disable icrnl"
ok -nl || pc_fail "stty -nl failed"
has nonl.out icrnl && has nonl.out -inlcr && has nonl.out -igncr || pc_fail "-nl did not give icrnl -inlcr -igncr"
ok ek || pc_fail "stty ek failed"
ok sane || pc_fail "stty sane failed"
pc_pass
