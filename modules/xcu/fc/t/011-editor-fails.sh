# A failing editor stops the command from running.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.011
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.011
# Run an interactive shell on a script of the commands given, one per
# line, with a history file of its own; its standard output is left in out.
runi() {
	hist=$1
	shift
	printf '%s\n' "$@" >script
	HISTFILE=$PWD/$hist ENV= "${PC_SH:-sh}" -i <script >out 2>err
}
tab=$(printf '\t')
mkdir bin || pc_fail "cannot make a directory"
printf 'printf "echo delta\\n" >"$1"\nexit 1\n' >bin/pcfail || pc_fail "cannot write the editor"
chmod +x bin/pcfail || pc_fail "cannot make the editor executable"
PATH=$PWD/bin:$PATH
pc_test
runi h1 'echo alpha' 'fc -e pcfail' 'exit 0'
a=0 d=0
while IFS= read -r line; do
	case $line in
	alpha) a=$((a + 1)) ;;
	delta) d=$((d + 1)) ;;
	esac
done <out
[ "$a" -eq 1 ] || pc_fail "alpha was written $a times, wanted 1"
[ "$d" -eq 0 ] || pc_fail "the command left by the failing editor was run"
pc_pass
