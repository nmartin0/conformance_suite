# Fixed order of the symbols, whatever the order of options.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uname.003
. "$PC_CORE/pc.sh"
pc_begin XCU.uname.003
LC_ALL=C; export LC_ALL
pc_test
s=$(uname -s) || pc_fail "uname -s failed"
n=$(uname -n) || pc_fail "uname -n failed"
r=$(uname -r) || pc_fail "uname -r failed"
v=$(uname -v) || pc_fail "uname -v failed"
m=$(uname -m) || pc_fail "uname -m failed"
chk() { # chk "options" "expected"
	x=$(uname $1) || pc_fail "uname $1 failed"
	[ "$x" = "$2" ] || pc_fail "uname $1 wrote '$x', expected '$2'"
}
chk "-ms" "$s $m"
chk "-sm" "$s $m"
chk "-nm" "$n $m"
chk "-mn" "$n $m"
chk "-vr" "$r $v"
chk "-rv" "$r $v"
chk "-nr" "$n $r"
chk "-rn" "$n $r"
chk "-vnsr" "$s $n $r $v"
chk "-s -n" "$s $n"
chk "-n -s" "$s $n"
chk "-mnrsv" "$s $n $r $v $m"
chk "-vsrnm" "$s $n $r $v $m"
pc_pass
