# When first is a newer command than last the commands are listed in reverse, as with -r.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.019
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.019
# Run an interactive shell on a script of the commands given, one per
# line, with a history file of its own; its standard output is left in out.
runi() {
	hist=$1
	shift
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/$hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
tab=$(printf '\t')
pc_test
runi h1 'echo e1' 'echo e2' 'echo e3' "fc -l -n -r 'echo e1' 'echo e3'" "fc -l -n 'echo e3' 'echo e1'" 'exit 0'
order=
while IFS= read -r line; do
	case $line in
	"${tab}echo e1") order=${order}1 ;;
	"${tab}echo e2") order=${order}2 ;;
	"${tab}echo e3") order=${order}3 ;;
	esac
done <out
[ "$order" = 321321 ] || pc_fail "the listings showed '$order', wanted 321321"
pc_pass
