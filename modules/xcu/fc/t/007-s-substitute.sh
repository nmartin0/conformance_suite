# fc -s old=new substitutes the first occurrence.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.007
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.007
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
runi h1 'echo alpha' 'fc -s alpha=gamma' 'exit 0'
a=0 g=0
while IFS= read -r line; do
	case $line in
	alpha) a=$((a + 1)) ;;
	gamma) g=$((g + 1)) ;;
	esac
done <out
[ "$a" -eq 1 ] || pc_fail "alpha was written $a times, wanted 1"
[ "$g" -eq 1 ] || pc_fail "gamma was written $g times, wanted 1 (the changed command)"
pc_pass
