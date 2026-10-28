# fc -l lists commands with numbers.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.001
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.001
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
runi h1 'echo alpha' 'echo beta' 'fc -l -2 -1' 'exit 0'
order= n1= n2=
while IFS= read -r line; do
	case $line in
	*"$tab"echo\ alpha) n1=${line%%"$tab"*}; order=${order}A ;;
	*"$tab"echo\ beta) n2=${line%%"$tab"*}; order=${order}B ;;
	esac
done <out
[ "$order" = AB ] || pc_fail "the listing has the commands in order '$order', wanted AB (oldest first), with a number, a tab and the command"
case $n1 in
''|*[!0-9]*) pc_fail "command number '$n1' is not a decimal integer" ;;
esac
case $n2 in
''|*[!0-9]*) pc_fail "command number '$n2' is not a decimal integer" ;;
esac
[ "$n1" != "$n2" ] || pc_fail "both commands have the number $n1"
pc_pass
