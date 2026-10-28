# PS1 undergoes parameter expansion each time the shell is ready to read a command and the result is written to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.020
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.020
LC_ALL=C; export LC_ALL
# Run an interactive shell on the script of commands given, one per line;
# its standard output is left in out and its standard error, where the
# prompts go, in err.
runi() {
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/pc.hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
pc_test
PS1='<${PCN}>'; export PS1
runi 'PCN=7' 'PCN=8' 'exit 0'
e=$(cat err); o=$(cat out)
for p in '<>' '<7>' '<8>'; do
	case $e in *"$p"*) : ;; *) pc_fail "the prompt $p is not in the error output: $e" ;; esac
done
case $o in *'<'*) pc_fail "a prompt reached standard output: $o" ;; esac
pc_pass
