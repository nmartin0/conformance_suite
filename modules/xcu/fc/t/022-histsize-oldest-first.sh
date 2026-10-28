# When HISTSIZE limits the commands that are accessible, the oldest commands are the ones lost.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.022
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.022
# Run an interactive shell on a script of the commands given, one per
# line, with a history file of its own; its standard output is left in out.
runi() {
	hist=$1
	shift
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/$hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
tab=$(printf '\t')
{ echo 'echo first'; i=0; while [ $i -lt 300 ]; do echo ':'; i=$((i + 1)); done; echo 'echo last'; echo 'fc -l -n 1 99999'; echo 'exit 0'; } >script || pc_fail "write"
pc_test
HISTSIZE=150; export HISTSIZE
HISTFILE=$PWD/h1 ENV= "${PC_SH:-sh}" -i <script >out 2>err
f=0 l=0 t=0
while IFS= read -r line; do
	case $line in
	"${tab}echo first") f=$((f + 1)) ;;
	"${tab}echo last") l=$((l + 1)) ;;
	esac
	case $line in *"$tab"*) t=$((t + 1)) ;; esac
done <out
[ "$l" -ge 1 ] || pc_fail "the newest command is not in the history"
[ "$f" -eq 0 ] || pc_fail "the oldest command is still listed after 300 commands with HISTSIZE=150"
[ "$t" -le 151 ] || pc_fail "$t commands were listed, more than the limit"
pc_pass
