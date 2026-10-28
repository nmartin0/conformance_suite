# Redirections and variable assignments on an fc command apply both to fc and to the command it re-executes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.016
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.016
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
runi h1 'echo toerr >&2' 'fc -s -- -1 2>/dev/null' 'echo "[$PCV]"' 'PCV=x fc -s -- -1' 'exit 0'
n=0
while IFS= read -r line; do
	case $line in *toerr*) n=$((n + 1)) ;; esac
done <err
[ "$n" -eq 1 ] || pc_fail "toerr reached standard error $n times, wanted once (the first run only)"
x=0 e=0
while IFS= read -r line; do
	case $line in
	'[x]') x=$((x + 1)) ;;
	'[]') e=$((e + 1)) ;;
	esac
done <out
[ "$e" -eq 1 ] || pc_fail "the line [] appeared $e times, wanted 1"
[ "$x" -eq 1 ] || pc_fail "the assignment before fc was not seen by the reinvoked command ([x] appeared $x times)"
pc_pass
