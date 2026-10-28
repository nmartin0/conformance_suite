# find -iname: the match is case-insensitive in the sense of the counterpart
# rule, for literals, bracket ranges and classes; -name stays sensitive.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.4.1-case-insensitive-comparisons.002
. "$PC_CORE/pc.sh"
pc_begin XBD.4.1-case-insensitive-comparisons.002
LC_ALL=C; export LC_ALL
mkdir d || pc_unresolved "mkdir d failed"
for n in READme.TXT beta CHARLIE Zulu k9; do
	: >"d/$n" || pc_unresolved "cannot create d/$n"
done
pc_test

# count PRIMARY PATTERN: number of names found by find d -type f PRIMARY PATTERN
count() {
	find d -type f "$1" "$2" 2>/dev/null | wc -l | tr -d ' \t'
}
chk() {
	n=$(count "$1" "$2")
	[ "$n" = "$3" ] || pc_fail "find $1 '$2' found $n names, expected $3"
}
chk -name  'readme.txt' 0
chk -iname 'readme.txt' 1
chk -iname 'README.TXT' 1
chk -iname 'readme.*'   1
chk -name  '[a-c]*'     1
chk -iname '[a-c]*'     2
chk -iname '[A-C]*'     2
chk -iname '[[:lower:]]ulu' 1
chk -iname '[[:upper:]]eta' 1
chk -iname 'K?'         1
chk -iname 'beta'       1
chk -iname 'betb'       0
pc_pass
