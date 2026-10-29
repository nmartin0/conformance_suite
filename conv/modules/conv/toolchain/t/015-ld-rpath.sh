# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION CONV.toolchain.015
#
# ld -rpath DIR records DIR in the dynamic section (as a run path or a
# library path entry); several -rpath options are joined with colons in
# command-line order; a $ORIGIN token is stored unexpanded.
. "$PC_CORE/pc.sh"
. "${PC_CORE%/core}/conv/lib/tc.sh"
tc_begin CONV.toolchain.015
tc_need_ld
printf 'int tcq(void) { return 1; }\n' >q.c
printf 'int tcq(void);\nint tcstart(void) { return tcq(); }\n' >m.c
tc_cobj q.c
tc_cobj m.c
tc_must "making the shared object" "$LD" -shared -o libtcq.so q.o
pc_test
tc_do "$LD" -e tcstart -o prog m.o -L . -ltcq -rpath /tc/first -rpath '$ORIGIN/../lib' || pc_fail "ld -rpath failed: $(tc_out)"
tc_dump prog
p=$(tc_dyn prog RUNPATH; tc_dyn prog RPATH)
p=$(printf '%s\n' "$p" | sed -n 1p)
[ -n "$p" ] || pc_fail "the program has neither a run path nor a library path entry"
[ "$p" = '/tc/first:$ORIGIN/../lib' ] || pc_fail "the recorded path is '$p', not '/tc/first:\$ORIGIN/../lib'"
pc_pass
