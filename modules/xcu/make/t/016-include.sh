# include lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.016
# include FILE reads FILE as if in place (also through a macro and nested);
# a missing file is an error.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.016
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
INC = inc.mk
include $(INC)
all:
	echo "[$(FROM_INC)] [$(FROM_NESTED)]"
EOM
printf 'FROM_INC = yes\ninclude nested.mk\n' >inc.mk
printf 'FROM_NESTED = deep\n' >nested.mk
cat >bad.mk <<'EOM'
.POSIX:
include missing.mk
all:
	echo reached
EOM
pc_test
make -s >o 2>err </dev/null || pc_fail "make: nonzero status"
[ "$(cat o)" = '[yes] [deep]' ] || pc_fail "included macros gave '$(cat o)'"
make -s -f bad.mk >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "include of a missing file gave status 0"
pc_pass
