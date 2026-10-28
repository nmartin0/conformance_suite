# Numeric option-arguments are decimal integers, up to 2147483647.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.1-utility-argument-syntax.002
#
# 08 would be invalid as an octal numeral and 010 would be eight, so the
# width and line count below tell a decimal reading from any other.
. "$PC_CORE/pc.sh"
pc_begin XBD.12.1-utility-argument-syntax.002
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'abcdefghij\n' >w || pc_unresolved "cannot create w"
: >l12 || pc_unresolved "cannot create l12"
i=1
while [ "$i" -le 12 ]; do
	printf 'line%s\n' "$i" >>l12 || pc_unresolved "cannot extend l12"
	i=$((i + 1))
done
printf 'nocolon\n' >nc || pc_unresolved "cannot create nc"
pc_test
chk() { # <what> <got> <expected>
	[ "$2" = "$3" ] || pc_fail "$1 gave '$2', expected '$3'"
}
chk "fold -w 08" "$(fold -w 08 w 2>/dev/null </dev/null)" "abcdefgh${NL}ij"
chk "fold -w 010" "$(fold -w 010 w 2>/dev/null </dev/null)" "abcdefghij"
chk "fold -w 9" "$(fold -w 9 w 2>/dev/null </dev/null)" "abcdefghi${NL}j"
chk "fold -w 2147483647" "$(fold -w 2147483647 w 2>/dev/null </dev/null)" "abcdefghij"
got=$(head -n 010 l12 2>/dev/null </dev/null)
want=$(head -n 10 l12 2>/dev/null </dev/null)
[ "$got" = "$want" ] || pc_fail "head -n 010 gave a different result from head -n 10"
n=0
while IFS= read -r l; do n=$((n + 1)); done <<EOT
$got
EOT
[ "$n" -eq 10 ] || pc_fail "head -n 010 of a 12-line file gave $n lines, expected 10"
chk "cut -d : -f 2147483647" "$(cut -d : -f 2147483647 nc 2>/dev/null </dev/null)" "nocolon"
pc_pass
