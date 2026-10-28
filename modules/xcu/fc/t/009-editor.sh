# fc -e runs the editor and then the edited command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.fc.009
. "$PC_CORE/pc.sh"
pc_begin XCU.fc.009
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
printf 'cat "$1" >seen\nprintf "echo delta\\n" >"$1"\n' >bin/pced || pc_fail "cannot write the editor"
chmod +x bin/pced || pc_fail "cannot make the editor executable"
PATH=$PWD/bin:$PATH
pc_test
runi h1 'echo alpha' 'fc -e pced' 'exit 0'
[ -f seen ] || pc_fail "the editor was not run"
found=
while IFS= read -r line; do
	[ "$line" = 'echo alpha' ] && found=1
done <seen
[ -n "$found" ] || pc_fail "the file given to the editor did not hold the command"
d=0
while IFS= read -r line; do
	[ "$line" = delta ] && d=$((d + 1))
done <out
[ "$d" -eq 1 ] || pc_fail "the edited command was run $d times, wanted once"
pc_pass
