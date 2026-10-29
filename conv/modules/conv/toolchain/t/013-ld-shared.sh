# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.013
#
# ld -shared turns position-independent objects into a shared object (type
# DYN with a dynamic section and the global symbols exported) that a
# program can open at run time and call into.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.013
tc_need_ld
tc_need_probe pic "-fPIC -shared"
tc_need_dl
printf 'int tcq(void) { return 42; }\nstatic int tchidden(void) { return 1; }\nint tcq2(void) { return tchidden(); }\n' >q.c
cat >main.c <<'EOT'
#include <dlfcn.h>
#include <stdio.h>
int main(int argc, char **argv)
{
	void *h;
	int (*f)(void);

	(void)argc;
	h = dlopen(argv[1], RTLD_NOW);
	if (h == NULL) { puts("dlopen failed"); return 2; }
	f = (int (*)(void))dlsym(h, "tcq");
	if (f == NULL) { puts("dlsym failed"); return 3; }
	printf("%d\n", f());
	if (dlsym(h, "tchidden") != NULL) { puts("static symbol visible"); return 4; }
	return dlclose(h) != 0 ? 5 : 0;
}
EOT
tc_cobj q.c -fPIC
tc_must "linking the program" "$CC" -o prog main.c $TC_DL
pc_test
tc_do "$LD" -shared -o libtcq.so q.o || pc_fail "ld -shared failed: $(tc_out)"
tc_dump libtcq.so
[ "$(tc_type libtcq.so)" = DYN ] || pc_fail "the output has type $(tc_type libtcq.so), not a shared object"
[ -n "$(awk '$1 == "SEC" && $4 == "DYNAMIC"' libtcq.so.dump)" ] || pc_fail "the shared object has no dynamic section"
l=$(tc_symdyn libtcq.so tcq)
[ -n "$l" ] && [ "$(tc_key "$l" shndx)" != UND ] || pc_fail "tcq is not exported by the shared object"
l=$(tc_symdyn libtcq.so tchidden)
[ -z "$l" ] || pc_fail "a static function is exported by the shared object"
out=$(./prog ./libtcq.so 2>&1)
rc=$?
[ "$rc" = 0 ] || pc_fail "opening the shared object and calling tcq failed (status $rc: $out)"
[ "$out" = 42 ] || pc_fail "tcq returned '$out' through the opened object, not 42"
pc_pass
