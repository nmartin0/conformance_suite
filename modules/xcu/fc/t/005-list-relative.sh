# Negative numbers and strings select commands; last defaults to the previous command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.005
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.005
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
runi h1 'echo alpha' 'true' 'fc -l -n -2' 'exit 0'
order=
while IFS= read -r line; do
	case $line in
	"${tab}echo alpha") order=${order}A ;;
	"${tab}true") order=${order}T ;;
	esac
done <out
[ "$order" = AT ] || pc_fail "fc -l -n -2 listed '$order', wanted AT (echo alpha, true)"
runi h2 'echo alpha' 'true' 'fc -l -n echo' 'exit 0'
order=
while IFS= read -r line; do
	case $line in
	"${tab}echo alpha") order=${order}A ;;
	"${tab}true") order=${order}T ;;
	esac
done <out
[ "$order" = AT ] || pc_fail "fc -l -n echo listed '$order', wanted AT (echo alpha, true)"
pc_pass
