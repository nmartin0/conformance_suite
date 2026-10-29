# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.005
#
# .comm NAME,SIZE[,ALIGN] declares a global common symbol: in the object it
# is defined relative to the common section index, its size is SIZE and its
# value is the alignment in bytes.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.005
tc_need AS CC
cat >a.s <<'EOT'
	.comm tcc1,24,16
	.comm tcc2,8
EOT
tc_aobj a.s
tc_dump a.o
pc_test
for n in tcc1 tcc2; do
	l=$(tc_sym a.o $n)
	[ -n "$l" ] || pc_fail "symbol $n missing"
	[ "$(tc_key "$l" shndx)" = COM ] || pc_fail "$n is not defined relative to the common section index (shndx $(tc_key "$l" shndx))"
	[ "$(tc_key "$l" bind)" = GLOBAL ] || pc_fail "$n is not global"
	case $(tc_key "$l" type) in OBJECT | COMMON | NOTYPE) ;; *) pc_fail "$n has type $(tc_key "$l" type)" ;; esac
done
l=$(tc_sym a.o tcc1)
[ "$(tc_key "$l" size)" = 24 ] || pc_fail "tcc1 has size $(tc_key "$l" size), not 24"
[ "$(tc_key "$l" value)" = 0x10 ] || pc_fail "tcc1 has value $(tc_key "$l" value) (alignment), not 0x10"
l=$(tc_sym a.o tcc2)
[ "$(tc_key "$l" size)" = 8 ] || pc_fail "tcc2 has size $(tc_key "$l" size), not 8"
pc_pass
