# Without -v od replaces groups of lines identical to the one before by a line of only an asterisk; -v writes everything.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.006
. "$PC_CORE/pc.sh"
pc_begin XCU.od.006
LC_ALL=POSIX
export LC_ALL
NL='
'
B=" $(printf '\t')"
a64=aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
data=
i=0
while [ $i -lt 16 ]; do
	data=$data$a64
	i=$((i + 1))
done
printf '%s' "$data" >f1024 || pc_fail "cannot create input"
pc_test
od -A d -t x1 f1024 >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "od gave status $rc"
star=no
n=0
while IFS= read -r line; do
	n=$((n + 1))
	[ "$line" = '*' ] && star=yes
	last=$line
done <out
[ "$star" = yes ] || pc_fail "no line with only an asterisk for 1024 identical bytes"
set -- $last
[ $# -eq 1 ] || pc_fail "the last line is not a single offset"
z=${1%%[!0]*}
off=${1#"$z"}
[ "$off" = 1024 ] || pc_fail "the last offset is not 1024"
od -A d -v -t x1 f1024 >out 2>/dev/null </dev/null
total=0
star=no
while IFS= read -r line; do
	[ "$line" = '*' ] && star=yes
	set -- $line
	[ $# -gt 1 ] && total=$((total + $# - 1))
done <out
[ "$star" = no ] || pc_fail "-v still wrote an asterisk line"
[ "$total" -eq 1024 ] || pc_fail "-v did not write all 1024 bytes"
pc_pass
