# fc -s string runs the most recent command starting with it.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.008
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.008
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
runi h1 'echo one' 'true' 'fc -s echo' 'exit 0'
n=0
while IFS= read -r line; do
	case $line in
	one) n=$((n + 1)) ;;
	esac
done <out
[ "$n" -eq 2 ] || pc_fail "one was written $n times, wanted 2 (the command and its re-run)"
pc_pass
