# Only the first -- ends the options; a second -- is an operand.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.006
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.006
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'DD\n' >./-- || pc_unresolved "cannot create a file named --"
printf 'EE\n' >e || pc_unresolved "cannot create e"
pc_test
chk() { # <what> <got> <expected>
	[ "$2" = "$3" ] || pc_fail "$1 gave '$2', expected '$3'"
}
chk "cat -- --" "$(cat -- -- 2>/dev/null </dev/null)" "DD"
chk "cat -- -- e" "$(cat -- -- e 2>/dev/null </dev/null)" "DD${NL}EE"
chk "head -n 1 -- --" "$(head -n 1 -- -- 2>/dev/null </dev/null)" "DD"
chk "ls -- --" "$(ls -- -- 2>/dev/null </dev/null)" "--"
set -- $(wc -l -- -- 2>/dev/null </dev/null)
[ "$#" -eq 2 ] && [ "$1" = 1 ] && [ "$2" = "--" ] || pc_fail "wc -l -- -- gave '$*', expected the count 1 and the name --"
pc_pass
