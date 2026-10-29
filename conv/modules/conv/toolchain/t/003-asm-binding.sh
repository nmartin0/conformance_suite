# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.003
#
# .globl makes a defined symbol global, .weak makes it weak, .local and the
# absence of any directive leave it local; in the symbol table every local
# symbol precedes the others and the table's info field names the first
# non-local one.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.003
tc_need AS CC
cat >a.s <<'EOT'
	.text
	.globl tcg1
	.weak tcw1
	.local tcl1
tcl1:	.byte 0
tcw1:	.byte 0
tcplain:	.byte 0
tcg1:	.byte 0
	.globl tcg2
tcg2:	.byte 0
EOT
tc_aobj a.s
tc_dump a.o
pc_test
want() { # NAME BINDING
	l=$(tc_sym a.o "$1")
	[ -n "$l" ] || pc_fail "symbol $1 missing"
	[ "$(tc_key "$l" bind)" = "$2" ] || pc_fail "symbol $1 has binding $(tc_key "$l" bind), not $2"
	[ "$(tc_key "$l" shndx)" != UND ] || pc_fail "symbol $1 is undefined"
}
want tcg1 GLOBAL
want tcg2 GLOBAL
want tcw1 WEAK
want tcl1 LOCAL
want tcplain LOCAL
# order: no LOCAL after the first non-LOCAL; sh_info is the index of that one
first=$(awk '$1 == "SYM" && $2 == ".symtab" && $0 !~ /bind=LOCAL/ { print $3; exit }' a.o.dump)
[ -n "$first" ] || pc_fail "no non-local symbol in the symbol table"
late=$(awk -v f="$first" '$1 == "SYM" && $2 == ".symtab" && $3 > f && $0 ~ /bind=LOCAL/ { print $4; exit }' a.o.dump)
[ -z "$late" ] || pc_fail "local symbol $late follows a non-local symbol in the symbol table"
info=$(awk '$1 == "SEC" && $4 == "SYMTAB" { print $8; exit }' a.o.dump)
[ "$info" = "$first" ] || pc_fail "the symbol table's info field is $info, the first non-local symbol is entry $first"
pc_pass
