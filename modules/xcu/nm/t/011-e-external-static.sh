# nm -e lists external and static symbols
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nm.011
. "$PC_CORE/pc.sh"
pc_begin XCU.nm.011
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v c17 >/dev/null 2>&1 || pc_notinuse "c17 is not available"
cat >x.c <<'EOC'
static int pclocal = 5;
int pcglobal = 7;
int pcfunc(void) { return pclocal + pcglobal; }
EOC
c17 -c x.c </dev/null >/dev/null 2>&1 || pc_unresolved "setup: c17 -c failed"
[ -f x.o ] || pc_unresolved "setup: c17 -c produced no x.o"
nm -P x.o >all 2>/dev/null </dev/null || pc_unresolved "setup: nm -P failed"
awk '$1 ~ /pclocal$/ { f = 1 } END { exit !f }' all || pc_unresolved "setup: the static variable is not in the object"
pc_test
nm -e -P x.o >out 2>err </dev/null || pc_fail "nm -e -P: nonzero status: $(cat err)"
for s in pcglobal pcfunc pclocal; do
	awk -v s="$s" '$1 ~ (s "$") { f = 1 } END { exit !f }' out || pc_fail "nm -e does not list $s: $(cat out)"
done
pc_pass
