# -p writes macros and targets
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.026
# -p writes the macro definitions and target descriptions to standard
# output (the format is not specified, so only names and values are looked
# for).
. "$PC_CORE/pc.sh"
pc_begin XCU.make.026
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
PCM = pcmacvalue
pctarget: pcdep
	echo building
pcdep:
	echo dep
EOM
pc_test
make -p -n >o 2>err </dev/null || pc_fail "make -p -n: nonzero status"
grep PCM o >/dev/null || pc_fail "the macro name PCM is not in the -p output"
grep pcmacvalue o >/dev/null || pc_fail "the macro value is not in the -p output"
grep pctarget o >/dev/null || pc_fail "the target name is not in the -p output"
grep pcdep o >/dev/null || pc_fail "the prerequisite name is not in the -p output"
pc_pass
