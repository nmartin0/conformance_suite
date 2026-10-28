# bg for a running background job exits 0.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.bg.003
. "$PC_CORE/pc.sh"
pc_begin XCU.bg.003
cat >script <<'PCS'
case $- in *m*) echo JC ;; *) echo NOJC ;; esac
sleep 101 &
bg %%
echo bgrc=$?
kill -s KILL $!
wait
exit 0
PCS
[ $? -eq 0 ] || pc_fail "cannot write the script"
pc_test
ENV= "${PC_SH:-sh}" -i <script >out 2>err
first=
bgrc=
while IFS= read -r line; do
	[ -n "$first" ] || first=$line
	case $line in
	bgrc=*) bgrc=${line#bgrc=} ;;
	esac
done <out
case $first in
JC) ;;
*) pc_unresolved "the interactive shell did not enable job control" ;;
esac
[ "$bgrc" = 0 ] || pc_fail "bg for a running job exited '$bgrc', wanted 0"
pc_pass
