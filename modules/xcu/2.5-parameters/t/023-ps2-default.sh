# The default value of PS2 is a greater-than sign followed by a space.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.023
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.023
LC_ALL=C; export LC_ALL
# Run an interactive shell on the script of commands given, one per line;
# its standard output is left in out and its standard error, where the
# prompts go, in err.
runi() {
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/pc.hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
pc_test
unset PS2
PS1='P#'; export PS1
runi 'echo "x' 'y"' 'exit 0'
case $(cat err) in *'> '*) : ;; *) pc_fail "no default continuation prompt in: $(cat err)" ;; esac
pc_pass
