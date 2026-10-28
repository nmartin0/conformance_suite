# fg brings a background job to the foreground in an interactive shell.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fg.002
. "$PC_CORE/pc.sh"
pc_begin XCU.fg.002
cat >script <<'PCS'
case $- in *m*) echo JC ;; *) echo NOJC ;; esac
sleep 2 &
fg
echo fgrc=$?
jobs
exit 0
PCS
[ $? -eq 0 ] || pc_fail "cannot write the script"
pc_test
ENV= "${PC_SH:-sh}" -i <script >out 2>err
first=
fgline= fgrc= listed=
while IFS= read -r line; do
	[ -n "$first" ] || first=$line
	case $line in
	"sleep 2"*) fgline=1 ;;
	fgrc=*) fgrc=${line#fgrc=} ;;
	"["*) listed=1 ;;
	esac
done <out
case $first in
JC) ;;
*) pc_unresolved "the interactive shell did not enable job control" ;;
esac
[ -n "$fgline" ] || pc_fail "fg did not write the command line"
[ "$fgrc" = 0 ] || pc_fail "the status after fg is '$fgrc', wanted 0 (the job's status)"
[ -z "$listed" ] || pc_fail "the job is still listed after fg"
pc_pass
