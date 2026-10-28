# end-of-file on standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.020
# End-of-file in command mode acts as q (status 0, nothing written) and
# end-of-file in input mode ends input; ed terminates and does not write the
# file.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.020
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
cat f >f.keep
pc_test
: >empty
ed -s f <empty >o1 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "EOF in command mode: exit status $rc, expected 0"
[ ! -s o1 ] || pc_fail "EOF in command mode wrote '$(cat o1)'"
printf '2p\n' | ed -s f >o2 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "EOF after a command: exit status $rc, expected 0"
[ "$(cat o2)" = two ] || pc_fail "got '$(cat o2)' for 2p before EOF"
printf 'a\nfoo\nbar\n' >in3
rm -f done
( ed -s f <in3 >o3 2>err; echo $? >done ) &
pid=$!
i=0
while [ ! -f done ] && [ "$i" -lt 30 ]; do sleep 1; i=$((i+1)); done
if [ ! -f done ]; then
	kill -9 "$pid" 2>/dev/null
	wait "$pid" 2>/dev/null
	pc_fail "ed did not end after end-of-file in input mode"
fi
wait "$pid" 2>/dev/null
cmp -s f f.keep || pc_fail "the file was modified although nothing wrote it"
pc_pass
