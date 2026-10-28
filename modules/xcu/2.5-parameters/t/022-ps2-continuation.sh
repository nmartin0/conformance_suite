# PS2 undergoes parameter expansion and is written to standard error each time a newline is entered before a command is complete.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.022
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.022
LC_ALL=C; export LC_ALL
# Run an interactive shell on the script of commands given, one per line;
# its standard output is left in out and its standard error, where the
# prompts go, in err.
runi() {
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/pc.hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
pc_test
PS1='P#'; PS2='${PCN}c> '; export PS1 PS2
runi 'PCN=Z' 'echo "x' 'y"' 'exit 0'
e=$(cat err)
case $e in *'Zc> '*) : ;; *) pc_fail "no expanded continuation prompt in: $e" ;; esac
n=$(grep -c 'Zc> ' err)
[ "$n" -eq 1 ] || pc_fail "the continuation prompt was written $n times"
pc_pass
