# kill %% signals the current job (job control on).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.kill.011
# MUTANT    kill.job_rejected
# MUTANT    kill.job_default_usr1
# MUTANT    kill.job_drops
# MUTANT    kill.job_n_shift
. "$PC_CORE/pc.sh"
pc_begin XCU.kill.011
p= q=
trap 'command kill -s KILL "$p" "$q" 2>/dev/null' EXIT
set -m || pc_fail "set -m is not accepted"
pc_test
sleep 30 &
p=$!
sleep 1
kill '%%' 2>/dev/null || pc_fail "kill %% failed"
wait "$p"
st=$?
n=$(command kill -l "$st")
[ "$n" = TERM ] || pc_fail "the job ended by '$n' after kill %%, wanted TERM"
# a job named by its number (the numbers are the shell's to choose: read
# them from the jobs listing, told apart by their commands)
sleep 30 &
p=$!
sleep 29 &
q=$!
sleep 1
jobs >jl 2>/dev/null || pc_unresolved "jobs failed, so the job numbers are unknown"
np=$(sed -n '/sleep 30/s/^\[\([0-9][0-9]*\)\].*/\1/p' jl)
nq=$(sed -n '/sleep 29/s/^\[\([0-9][0-9]*\)\].*/\1/p' jl)
case $np:$nq in
*[!0-9:]*|:*|*:) pc_unresolved "the job numbers could not be read from the jobs listing" ;;
esac
[ "$np" != "$nq" ] || pc_unresolved "two jobs with one number in the jobs listing"
kill -s USR1 "%$np" 2>/dev/null || pc_fail "kill -s USR1 %$np failed"
wait "$p"
n=$(command kill -l "$?")
[ "$n" = USR1 ] || pc_fail "the first job ended by '$n' after kill -s USR1 %$np"
kill -s USR2 "%$nq" 2>/dev/null || pc_fail "kill -s USR2 %$nq failed"
wait "$q"
n=$(command kill -l "$?")
[ "$n" = USR2 ] || pc_fail "the second job ended by '$n' after kill -s USR2 %$nq"
pc_pass
