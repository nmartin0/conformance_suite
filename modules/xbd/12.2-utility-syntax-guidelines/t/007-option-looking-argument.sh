# An argument that can be identified as an option is one, even if a file of that name exists.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.007
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.007
LC_ALL=POSIX
export LC_ALL
printf 'FILE-U\n' >./-u || pc_unresolved "cannot create the file -u"
printf 'FILE-D\n' >./-d || pc_unresolved "cannot create the file -d"
printf 'FILE-L\n' >./-l || pc_unresolved "cannot create the file -l"
printf 'FILE-C\n' >./-c || pc_unresolved "cannot create the file -c"
pc_test
chk() { # <what> <got> <expected>
	[ "$2" = "$3" ] || pc_fail "$1 gave '$2', expected '$3'"
}
chk "cat -u" "$(printf 'STDIN\n' | cat -u 2>/dev/null)" "STDIN"
chk "ls -d" "$(ls -d 2>/dev/null </dev/null)" "."
set -- $(printf 'a\nb\n' | wc -l 2>/dev/null)
[ "$#" -eq 1 ] && [ "$1" = 2 ] || pc_fail "wc -l on standard input gave '$*', expected the count 2 alone"
chk "grep -c x" "$(printf 'x\nx\ny\n' | grep -c x 2>/dev/null)" "2"
pc_pass
