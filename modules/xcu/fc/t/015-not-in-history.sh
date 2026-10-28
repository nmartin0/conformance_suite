# The fc command that caused editing is not entered into the history list, while the edited command is.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.015
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.015
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
printf 'printf "echo delta\\n" >"$1"\n' >bin/pced || pc_fail "cannot write the editor"
chmod +x bin/pced || pc_fail "cannot make the editor executable"
PATH=$PWD/bin:$PATH
pc_test
runi h1 'echo alpha' 'fc -e pced' 'fc -l -n -10' 'exit 0'
a=0 d=0 f=0
while IFS= read -r line; do
	case $line in
	"${tab}echo alpha") a=$((a + 1)) ;;
	"${tab}echo delta") d=$((d + 1)) ;;
	"${tab}fc -e pced") f=$((f + 1)) ;;
	esac
done <out
[ "$a" -ge 1 ] || pc_fail "echo alpha is not in the history"
[ "$d" -ge 1 ] || pc_fail "the edited command is not in the history"
[ "$f" -eq 0 ] || pc_fail "the fc command that caused the editing was entered into the history"
pc_pass
