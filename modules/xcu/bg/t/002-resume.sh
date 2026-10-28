# bg resumes a suspended job in an interactive shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bg.002
. "$PC_CORE/pc.sh"
pc_begin XCU.bg.002
cat >script <<'PCS'
case $- in *m*) echo JC ;; *) echo NOJC ;; esac
sleep 101 &
kill -s TSTP $!
sleep 1
bg
echo bgrc=$?
jobs
kill -s KILL $!
wait
exit 0
PCS
[ $? -eq 0 ] || pc_fail "cannot write the script"
pc_test
ENV= "${PC_SH:-sh}" -i <script >out 2>err
first=
bgline= bgrc= running=
while IFS= read -r line; do
	[ -n "$first" ] || first=$line
	case $line in
	"["[0-9]*"] sleep 101"*) bgline=1 ;;
	bgrc=*) bgrc=${line#bgrc=} ;;
	"["[0-9]*"] + Running sleep 101"*) running=1 ;;
	esac
done <out
case $first in
JC) ;;
*) pc_unresolved "the interactive shell did not enable job control" ;;
esac
[ "$bgrc" = 0 ] || pc_fail "bg exited '$bgrc', wanted 0"
[ -n "$bgline" ] || pc_fail "bg did not write a line [n] sleep 101"
[ -n "$running" ] || pc_fail "jobs did not report the resumed job as Running"
pc_pass
