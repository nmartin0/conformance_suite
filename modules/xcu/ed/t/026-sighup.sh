# SIGHUP with a changed buffer
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ed.026
# On SIGHUP with a changed, non-empty buffer ed writes a copy of the buffer
# to ed.hup in the working directory (or the home directory) and exits without
# writing the remembered file.
. "$PC_CORE/pc.sh"
pc_begin XCU.ed.026
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\nfour\nfive\n' >f || pc_unresolved "setup: cannot write f"
mkdir home || pc_unresolved "cannot make a home directory"
cat f >f.keep
mkfifo in || pc_unresolved "cannot make a fifo"
pid=
cleanup() {
	[ -n "$pid" ] && kill -9 "$pid" 2>/dev/null
	exec 3>&-
	cd "$pc_top" && rm -rf "$pc_wd"
}
trap cleanup EXIT
pc_test
HOME=$pc_top/$pc_wd/home
export HOME
ed -s f <in >out 2>err &
pid=$!
exec 3>in
printf '$a\nadded\n.\n$p\n' >&3
i=0
while [ ! -s out ] && [ "$i" -lt 20 ]; do sleep 1; i=$((i+1)); done
[ -s out ] || pc_unresolved "ed did not answer a command"
kill -HUP "$pid" 2>/dev/null
i=0
while kill -0 "$pid" 2>/dev/null && [ "$i" -lt 20 ]; do
	# a zombie still answers kill -0, so reap by waiting when it is done
	[ -f ed.hup ] || [ -f home/ed.hup ] && break
	sleep 1; i=$((i+1))
done
sleep 1
kill -9 "$pid" 2>/dev/null
wait "$pid" 2>/dev/null
pid=
if [ -f ed.hup ]; then h=ed.hup; elif [ -f home/ed.hup ]; then h=home/ed.hup
else pc_fail "no ed.hup after SIGHUP"; fi
cat f.keep >exp
echo added >>exp
cmp -s "$h" exp || pc_fail "ed.hup holds '$(tr '\n' ' ' <"$h")', expected the buffer"
cmp -s f f.keep || pc_fail "the remembered file was written after SIGHUP"
pc_pass
