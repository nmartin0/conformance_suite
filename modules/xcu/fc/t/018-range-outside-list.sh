# A first or last value outside the history list is not an error: the oldest or newest command is used instead.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.018
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.018
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
runi h1 'echo r1' 'echo r2' 'echo r3' 'fc -l -n -99 -1' 'fc -l -n 1 99999' 'exit 0'
order=
while IFS= read -r line; do
	case $line in
	"${tab}echo r1") order=${order}1 ;;
	"${tab}echo r2") order=${order}2 ;;
	"${tab}echo r3") order=${order}3 ;;
	esac
done <out
[ "$order" = 123123 ] || pc_fail "the listings showed '$order', wanted 123123"
pc_pass
