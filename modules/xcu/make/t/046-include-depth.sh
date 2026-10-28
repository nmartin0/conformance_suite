# nested include files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.046
# Include files can be nested sixteen deep.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.046
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
i=1
while [ "$i" -le 16 ]; do
	n=$((i + 1))
	{
		[ "$i" -lt 16 ] && printf 'include f%d.mk\n' "$n"
		printf 'PCD%d = x\n' "$i"
	} >f$i.mk
	i=$n
done
printf '.POSIX:\ninclude f1.mk\nall:\n\t@echo "$(PCD1)$(PCD16)"\n' >makefile
[ -f f16.mk ] || pc_unresolved "setup: the include files were not written"
pc_test
make >o 2>err </dev/null || pc_fail "make: nonzero status: $(cat err)"
grep -Fx xx o >/dev/null || pc_fail "sixteen nested include files were not all read: $(cat o)"
pc_pass
