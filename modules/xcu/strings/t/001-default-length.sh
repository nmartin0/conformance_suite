# strings -a writes each run of four or more printable characters ended by a newline or NUL, one per line, and no shorter run.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.strings.001
. "$PC_CORE/pc.sh"
pc_begin XCU.strings.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abc\000defgh\000ij\000klmn\nxyz\n' >in || pc_fail "cannot create input"
pc_test
out=$(strings -a in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
case $out in
*rc=0) ;;
*) pc_fail "strings -a gave a nonzero status" ;;
esac
want=0
while IFS= read -r line; do
	case $line in
	defgh) [ $want -eq 0 ] && want=1 ;;
	klmn) [ $want -eq 1 ] && want=2 ;;
	abc | ij | xyz) pc_fail "a string shorter than four characters was written" ;;
	esac
done <<EOT
$(strings -a in 2>/dev/null </dev/null)
EOT
[ $want -eq 2 ] || pc_fail "defgh and klmn were not both written, in order, as whole lines"
pc_pass
