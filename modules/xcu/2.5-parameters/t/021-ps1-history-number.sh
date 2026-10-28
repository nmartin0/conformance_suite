# In PS1 an exclamation mark is replaced by the history number of the next command and two exclamation marks give one.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.5-parameters.021
. "$PC_CORE/pc.sh"
pc_begin XCU.2.5-parameters.021
LC_ALL=C; export LC_ALL
# Run an interactive shell on the script of commands given, one per line;
# its standard output is left in out and its standard error, where the
# prompts go, in err.
runi() {
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/pc.hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
pc_test
PS1='[!]'; export PS1
runi ':' ':' ':' 'exit 0'
nums=$(grep -o '\[[0-9][0-9]*\]' err | tr -d '[]' | tr '\n' ' ')
set -- $nums
[ $# -ge 3 ] || pc_fail "fewer than three numbered prompts in: $(cat err)"
a=$1; b=$2; c=$3
[ "$b" -eq $((a + 1)) ] && [ "$c" -eq $((b + 1)) ] || pc_fail "the numbers $a $b $c do not count the commands"
PS1='a!!b'; export PS1
runi ':' 'exit 0'
case $(cat err) in *'a!b'*) : ;; *) pc_fail "no literal exclamation mark in the prompt: $(cat err)" ;; esac
case $(cat err) in *'a!!b'*) pc_fail "the doubled exclamation mark was not reduced: $(cat err)" ;; esac
pc_pass
