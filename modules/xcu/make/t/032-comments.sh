# comments in a makefile
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.032
# A number sign starts a comment that is not part of a macro value or of the
# prerequisites; blank lines and lines starting with # are skipped.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.032
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
cat >makefile <<'EOM'
.POSIX:
# a comment line

PCV = value # trailing comment
PCW = other#nospace
all: pcdep # not a prerequisite
	@echo "[$(PCV)] [$(PCW)]"
pcdep:
	@echo dep-made
EOM
pc_test
make >o 2>err </dev/null || pc_fail "make: nonzero status"
grep -x dep-made o >/dev/null || pc_fail "the prerequisite before the comment was not made"
grep -E '^\[value *\] \[other\]$' o >/dev/null || pc_fail "a comment became part of a macro value: $(cat o)"
pc_pass
