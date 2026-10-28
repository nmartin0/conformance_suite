# macro operands on the command line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.044
# Macro definition operands, in both the delayed and the immediate form, are
# processed in the order given, a later one replacing an earlier one of the
# same source.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.044
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '.POSIX:\nall:\n\t@echo "[$(Y)] [$(Z)] [$(X)]"\n' >makefile || pc_unresolved "setup: cannot write the makefile"
pc_test
make X=a Y='$(X)' Z::='$(X)' X=b >o 2>err </dev/null || pc_fail "make: nonzero status"
grep -Fx '[b] [a] [b]' o >/dev/null || pc_fail "macro operands: $(cat o)"
pc_pass
