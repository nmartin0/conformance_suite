# macros from command line, environment and makefile
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.010
# Macros come from the command line, MAKEFLAGS, the environment, the
# makefile; a makefile definition overrides the environment unless -e is
# given; a command-line definition overrides both; a null environment value
# overrides with -e.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
V = file
all:
	echo "[$(V)]"
EOM
pc_test
V=env make -s >o 2>err </dev/null || pc_fail "make with V in the environment: nonzero status"
[ "$(cat o)" = '[file]' ] || pc_fail "without -e the makefile value did not win: $(cat o)"
V=env make -s -e >o 2>err </dev/null || pc_fail "make -e: nonzero status"
[ "$(cat o)" = '[env]' ] || pc_fail "with -e the environment value did not win: $(cat o)"
V=env make -s V=cmd >o 2>err </dev/null || pc_fail "make V=cmd: nonzero status"
[ "$(cat o)" = '[cmd]' ] || pc_fail "the command-line value did not win: $(cat o)"
V=env make -s -e V=cmd >o 2>err </dev/null || pc_fail "make -e V=cmd: nonzero status"
[ "$(cat o)" = '[cmd]' ] || pc_fail "with -e the command-line value did not win: $(cat o)"
V= make -s -e >o 2>err </dev/null || pc_fail "make -e with a null V: nonzero status"
[ "$(cat o)" = '[]' ] || pc_fail "a null environment value did not override with -e: $(cat o)"
pc_pass
