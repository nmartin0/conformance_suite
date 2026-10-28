# A command number as first and last selects that command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.004
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.004
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
runi h1 'echo alpha' 'echo beta' 'fc -l -2 -1 >nums' 'read n1 rest <nums' 'fc -l $n1 $n1' 'exit 0'
IFS= read -r numline <nums
want=${numline%%"$tab"*}
case $want in
''|*[!0-9]*) pc_fail "the first line of the listing has no command number: '$numline'" ;;
esac
a=0 b=
while IFS= read -r line; do
	case $line in
	"$want${tab}echo alpha") a=$((a + 1)) ;;
	*"${tab}echo beta") b=1 ;;
	esac
done <out
[ "$a" = 1 ] || pc_fail "fc -l $want $want did not list the command echo alpha exactly once"
[ -z "$b" ] || pc_fail "fc -l $want $want also listed echo beta"
pc_pass
