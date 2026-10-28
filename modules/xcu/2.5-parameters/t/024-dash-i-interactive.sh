# The -i option is included in the value of $- when the shell is interactive, whether or not it was given on invocation.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.024
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.024
LC_ALL=C; export LC_ALL
# Run an interactive shell on the script of commands given, one per line;
# its standard output is left in out and its standard error, where the
# prompts go, in err.
runi() {
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/pc.hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
pc_test
runi 'echo "flags:$-"' 'exit 0'
l=$(grep '^flags:' out | head -n 1)
[ -n "$l" ] || pc_unresolved "no flags line"
case ${l#flags:} in *i*) : ;; *) pc_fail "\$- is '${l#flags:}' without i" ;; esac
pc_pass
