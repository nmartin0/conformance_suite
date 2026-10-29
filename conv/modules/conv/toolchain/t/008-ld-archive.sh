# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.008
#
# An archive member is taken into the link only when it defines a symbol
# that is undefined at the point where the archive appears on the command
# line (so an archive placed before the object that needs it does not
# satisfy it); a weak undefined reference does not pull a member in.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.008
tc_need_ld AS
printf 'int tca(void);\nint tcstart(void) { return tca(); }\n' >m.c
printf 'int tca(void) { return 1; }\n' >a.c
printf 'int tcb(void) { return 2; }\n' >b.c
cat >w.s <<'EOT'
	.text
	.globl tcstart
tcstart:	.byte 0
	.weak tcwk
EOT
printf 'int tcwk(void) { return 3; }\n' >wk.c
for f in m a b wk; do tc_cobj $f.c; done
tc_aobj w.s
tc_must "making the archive" "$AR" -rc lib.a a.o b.o
tc_must "making the second archive" "$AR" -rc libw.a wk.o
pc_test
tc_do "$LD" -e tcstart -o o1 m.o lib.a || pc_fail "link with the archive after the object failed: $(tc_out)"
tc_dump o1
l=$(tc_sym o1 tca)
[ -n "$l" ] && [ "$(tc_key "$l" shndx)" != UND ] || pc_fail "tca, undefined in the object before the archive, was not taken from the archive"
l=$(tc_sym o1 tcb)
[ -z "$l" ] || pc_fail "the member that defines only tcb was taken although tcb was never undefined"
if tc_do "$LD" -e tcstart -o o2 lib.a m.o; then
	pc_fail "an archive placed before the object that needs it satisfied the reference"
fi
tc_do "$LD" -e tcstart -o o3 w.o libw.a || pc_fail "link with a weak undefined reference failed: $(tc_out)"
tc_dump o3
l=$(tc_sym o3 tcwk)
if [ -n "$l" ] && [ "$(tc_key "$l" shndx)" != UND ]; then
	pc_fail "a weak undefined reference pulled the member that defines tcwk out of the archive"
fi
pc_pass
