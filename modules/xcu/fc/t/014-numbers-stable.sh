# Commands in the history list are referenced by number; the number of a command does not change while later commands are added, and later commands have larger numbers.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.014
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.014
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
runi h1 'echo n1' 'echo n2' "fc -l 'echo n1' 'echo n2'" 'echo n3' "fc -l 'echo n1' 'echo n2'" 'exit 0'
k=0 a1= a2= b1= b2=
while IFS= read -r line; do
	case $line in
	*"$tab"echo\ n1) k=$((k + 1)); if [ $k -eq 1 ]; then a1=${line%%"$tab"*}; else b1=${line%%"$tab"*}; fi ;;
	*"$tab"echo\ n2) if [ -z "$a2" ]; then a2=${line%%"$tab"*}; else b2=${line%%"$tab"*}; fi ;;
	esac
done <out
for v in "$a1" "$a2" "$b1" "$b2"; do
	case $v in ''|*[!0-9]*) pc_fail "missing or bad number '$v' (got $a1 $a2 $b1 $b2)" ;; esac
done
[ "$a1" -eq "$b1" ] || pc_fail "echo n1 was number $a1, later $b1"
[ "$a2" -eq "$b2" ] || pc_fail "echo n2 was number $a2, later $b2"
[ "$a2" -gt "$a1" ] || pc_fail "the later command has the number $a2, not above $a1"
pc_pass
