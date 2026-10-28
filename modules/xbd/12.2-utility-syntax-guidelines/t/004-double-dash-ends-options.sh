# The first -- ends the options; later arguments are operands even when they start with a hyphen-minus.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.004
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.004
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'N1\nN2\n' >./-n || pc_unresolved "cannot create the file -n"
printf 'X1\n' >./-x || pc_unresolved "cannot create the file -x"
printf 'D1\n' >./-d || pc_unresolved "cannot create the file -d"
printf 'a-xb\nplain\n' >pf || pc_unresolved "cannot create pf"
pc_test
chk() { # <what> <got> <expected>
	[ "$2" = "$3" ] || pc_fail "$1 gave '$2', expected '$3'"
}
chk "cat -- -n" "$(cat -- -n 2>/dev/null </dev/null)" "N1${NL}N2"
chk "head -n 1 -- -n" "$(head -n 1 -- -n 2>/dev/null </dev/null)" "N1"
chk "tail -n 1 -- -n" "$(tail -n 1 -- -n 2>/dev/null </dev/null)" "N2"
chk "sort -- -n" "$(sort -- -n 2>/dev/null </dev/null)" "N1${NL}N2"
chk "cut -f 1 -- -n" "$(cut -f 1 -- -n 2>/dev/null </dev/null)" "N1${NL}N2"
chk "sed -n -- 1p -n" "$(sed -n -- 1p -n 2>/dev/null </dev/null)" "N1"
chk "grep -- N1 -n" "$(grep -- N1 -n 2>/dev/null </dev/null)" "N1"
chk "grep -- -x pf" "$(grep -- -x pf 2>/dev/null </dev/null)" "a-xb"
chk "ls -- -d" "$(ls -- -d 2>/dev/null </dev/null)" "-d"
set -- $(wc -l -- -n 2>/dev/null </dev/null)
[ "$#" -eq 2 ] && [ "$1" = 2 ] && [ "$2" = -n ] || pc_fail "wc -l -- -n gave '$*', expected the count 2 and the name -n"
mkdir -- -m 2>/dev/null </dev/null || pc_fail "mkdir -- -m failed"
[ -d ./-m ] || pc_fail "mkdir -- -m did not create a directory named -m"
rm -- -x 2>/dev/null </dev/null || pc_fail "rm -- -x failed"
[ ! -e ./-x ] || pc_fail "rm -- -x did not remove the file named -x"
pc_pass
