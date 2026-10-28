# fc -s runs the previous command again.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.006
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.006
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
runi h1 'echo alpha' 'fc -s' 'exit 0'
n=0
while IFS= read -r line; do
	case $line in
	alpha) n=$((n + 1)) ;;
	esac
done <out
[ "$n" -eq 2 ] || pc_fail "alpha was written $n times, wanted 2 (the command and its re-run)"
pc_pass
