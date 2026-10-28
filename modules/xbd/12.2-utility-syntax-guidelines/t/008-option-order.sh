# The order of different options relative to one another does not matter.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.008
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.008
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'Apple\nbanana\napple\n' >f || pc_unresolved "cannot create f"
printf 'a:b\nnodelim\nc:d\n' >g || pc_unresolved "cannot create g"
printf '2\n10\n1\n' >n || pc_unresolved "cannot create n"
printf 'one\ntwo\nthree\n' >t || pc_unresolved "cannot create t"
mkdir d || pc_unresolved "cannot create d"
pc_test
same() { # <what> <expected> <got>... : every <got> must equal <expected>
	w=$1
	e=$2
	shift 2
	for x in "$@"; do
		[ "$x" = "$e" ] || pc_fail "$w: got '$x', expected '$e'"
	done
}
e=$(ls -l -d d 2>/dev/null </dev/null)
case $e in
d*" d") ;;
*) pc_fail "ls -l -d d did not describe the directory: '$e'" ;;
esac
same "ls -l -d d" "$e" "$(ls -d -l d 2>/dev/null </dev/null)" "$(ls -ld d 2>/dev/null </dev/null)" "$(ls -dl d 2>/dev/null </dev/null)"
e=$(wc -l -w t 2>/dev/null </dev/null)
set -- $e
[ "$#" -eq 3 ] && [ "$1" = 3 ] && [ "$2" = 3 ] && [ "$3" = t ] || pc_fail "wc -l -w t gave '$e'"
same "wc -l -w t" "$e" "$(wc -w -l t 2>/dev/null </dev/null)" "$(wc -lw t 2>/dev/null </dev/null)" "$(wc -wl t 2>/dev/null </dev/null)"
want="1:Apple${NL}3:apple"
same "grep -n -i apple f" "$want" "$(grep -n -i apple f 2>/dev/null </dev/null)" "$(grep -i -n apple f 2>/dev/null </dev/null)" "$(grep -in apple f 2>/dev/null </dev/null)"
want="10${NL}2${NL}1"
same "sort -n -r n" "$want" "$(sort -n -r n 2>/dev/null </dev/null)" "$(sort -r -n n 2>/dev/null </dev/null)" "$(sort -rn n 2>/dev/null </dev/null)"
want="a${NL}c"
same "cut -s -f 1 -d : g" "$want" "$(cut -s -f 1 -d : g 2>/dev/null </dev/null)" "$(cut -f 1 -s -d : g 2>/dev/null </dev/null)" "$(cut -d : -f 1 -s g 2>/dev/null </dev/null)" "$(cut -d : -s -f 1 g 2>/dev/null </dev/null)"
pc_pass
