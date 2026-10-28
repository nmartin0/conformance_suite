# when macros are expanded; indirection depth
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.034
# Macros in a target line are expanded when the line is read, those in
# commands when they run, a macro in the name part of a definition is
# expanded at the definition, and at least 100 levels of indirection work.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.034
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
X = a
t: $(X)
X = b
N = Y
$(N) = val
all: t
	@echo "X=$(X) Y=$(Y)"
a: ; @echo a-built
b: ; @echo b-built
t: ; @echo t-built
EOM
{
	printf '.POSIX:\nA0 = deep\n'
	i=1
	while [ "$i" -le 100 ]; do printf 'A%d = $(A%d)\n' "$i" $((i - 1)); i=$((i + 1)); done
	printf 'all:\n\t@echo $(A100)\n'
} >deep.mk
pc_test
make all >o 2>err </dev/null || pc_fail "make all: nonzero status"
grep -x a-built o >/dev/null || pc_fail "the prerequisite named by X when the target line was read was not built"
grep -x b-built o >/dev/null && pc_fail "the target line saw the later value of X"
grep -Fx 'X=b Y=val' o >/dev/null || pc_fail "command-time or name-part expansion is wrong: $(cat o)"
make -f deep.mk >o 2>err </dev/null || pc_fail "make -f deep.mk: nonzero status"
grep -x deep o >/dev/null || pc_fail "100 levels of macro indirection were not followed: $(cat o)"
pc_pass
