# fc -l -r and a reversed range list newest first.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.003
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.003
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
runi h1 'echo alpha' 'echo beta' 'fc -l -n -r -2 -1' 'exit 0'
order=
while IFS= read -r line; do
	case $line in
	"${tab}echo alpha") order=${order}A ;;
	"${tab}echo beta") order=${order}B ;;
	esac
done <out
[ "$order" = BA ] || pc_fail "fc -l -n -r listed '$order', wanted BA"
runi h2 'echo alpha' 'echo beta' 'fc -l -n -1 -2' 'exit 0'
order=
while IFS= read -r line; do
	case $line in
	"${tab}echo alpha") order=${order}A ;;
	"${tab}echo beta") order=${order}B ;;
	esac
done <out
[ "$order" = BA ] || pc_fail "fc -l -n -1 -2 listed '$order', wanted BA"
pc_pass
