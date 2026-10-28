# suffix rules
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.014
# With .SUFFIXES listing .in and .out, a .in.out rule builds x.out from an
# existing x.in with $< and $* set accordingly and rebuilds when x.in is
# newer; a single-suffix rule builds tool from tool.one; an empty .SUFFIXES
# clears the list so the rule is no longer used; -r removes the built-in
# rules.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.014
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
.SUFFIXES: .in .out .one
.in.out:
	cp $< $@; echo "R $< $@ $*"
.one:
	cp $< $@; echo "S $< $@"
EOM
cat >clear.mk <<'EOM'
.POSIX:
.SUFFIXES:
.in.out:
	cp $< $@
EOM
printf 'data\n' >x.in; printf 'tool\n' >tool.one; printf 'int x;\n' >a.c
touch -t 200001010000 x.in || pc_unresolved "setup: touch failed"
pc_test
make -s x.out >o 2>err </dev/null || pc_fail "make x.out: nonzero status"
[ "$(cat x.out)" = data ] || pc_fail "the suffix rule did not build x.out"
[ "$(cat o)" = 'R x.in x.out x' ] || pc_fail "rule macros gave '$(cat o)'"
make -s x.out >o 2>err </dev/null || pc_fail "make x.out again: nonzero status"
[ -s o ] && pc_fail "an up-to-date target was rebuilt through the suffix rule"
printf 'newer\n' >x.in
touch -t 203001010000 x.in || pc_unresolved "touch failed"
make -s x.out >o 2>err </dev/null || pc_fail "make x.out after a change: nonzero status"
[ "$(cat x.out)" = newer ] || pc_fail "a newer source did not cause a rebuild"
make -s tool >o 2>err </dev/null || pc_fail "make tool: nonzero status"
[ "$(cat tool)" = tool ] || pc_fail "the single-suffix rule did not build tool"
rm -f x.out
make -s -f clear.mk x.out >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "after clearing .SUFFIXES the rule was still used"
make -r -s a.o >o 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "-r: a built-in rule built a.o"
pc_pass
