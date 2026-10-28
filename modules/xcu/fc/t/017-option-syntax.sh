# fc follows the utility syntax guidelines: options may be grouped in any order and a double hyphen ends the options so that a negative number is an operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.017
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.017
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
runi h1 'echo g1' 'echo g2' "fc -nl 'echo g1' 'echo g2'" "fc -ln 'echo g1' 'echo g2'" 'fc -l -n -- -2 -1 >/dev/null' 'echo "st=$?"' 'exit 0'
a=0 b=0
while IFS= read -r line; do
	case $line in
	"${tab}echo g1") a=$((a + 1)) ;;
	"${tab}echo g2") b=$((b + 1)) ;;
	esac
done <out
[ "$a" -ge 2 ] && [ "$b" -ge 2 ] || pc_fail "the grouped options did not list the commands without numbers (g1 $a, g2 $b)"
grep -x -F 'st=0' out >/dev/null || pc_fail "the double hyphen before negative numbers was not accepted: $(cat out)"
pc_pass
