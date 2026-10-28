# >>, <<, >| and >& in the command string
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.uux.001
# A command string using the redirection operators >>, <<, >| or >& is
# rejected with a non-zero exit status.
. "$PC_CORE/pc.sh"
pc_begin XCU.uux.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
pc_test
uux 'true >> pcuux.out' </dev/null >out 2>err && pc_fail "uux accepted the >> operator"
uux 'cat << pcuux' </dev/null >out 2>err && pc_fail "uux accepted the << operator"
uux 'true >| pcuux.out' </dev/null >out 2>err && pc_fail "uux accepted the >| operator"
uux 'true >& pcuux.out' </dev/null >out 2>err && pc_fail "uux accepted the >& operator"
pc_pass
