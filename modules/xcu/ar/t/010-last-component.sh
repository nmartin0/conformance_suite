# members are named by the last pathname component
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.010
# A file added as sub/f3 is stored under the name f3: "ar -t a"
# lists f3, and a later "ar -d a other/f3" (same last component)
# removes it.
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.010
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir sub other deep deep/er || pc_unresolved "setup: mkdir failed"
printf 'one\n' >f1; printf 'three\n' >sub/f3; printf 'other three\n' >other/f3
printf 'g\n' >sub/g; printf 'other g\n' >other/g
pc_test
ar -rc a f1 sub/f3 sub/g >out 2>err </dev/null || pc_fail "ar -r with a path operand: nonzero status"
[ "$(ar -t a </dev/null | sort | tr '\n' ' ')" = 'f1 f3 g ' ] || pc_fail "the members are not stored under the last component"
# A later operand with the same last component but another directory
# replaces the member rather than adding another one.
ar -r a other/g >out 2>err </dev/null || pc_fail "ar -r other/g: nonzero status"
[ "$(ar -t a </dev/null | sort | tr '\n' ' ')" = 'f1 f3 g ' ] || pc_fail "ar -r other/g did not replace the member g"
ar -p a g >out 2>/dev/null </dev/null
cmp -s out other/g || pc_fail "member g was not replaced by other/g"
# Every operation compares the last component of the operand.
ar -p a other/f3 >out 2>err </dev/null || pc_fail "ar -p a other/f3: nonzero status"
[ -f out ] || pc_fail "no output file"
cmp -s out sub/f3 || pc_fail "ar -p a other/f3 did not write member f3"
mkdir x || pc_unresolved "setup: mkdir failed"
( cd x && ar -x ../a deep/er/f3 >out 2>err </dev/null ) || pc_fail "ar -x a deep/er/f3: nonzero status"
[ -f x/f3 ] || pc_fail "ar -x a deep/er/f3 did not extract f3"
cmp -s x/f3 sub/f3 || pc_fail "ar -x a deep/er/f3 extracted the wrong contents"
ar -d a other/f3 >out 2>err </dev/null || pc_fail "ar -d with a different directory: nonzero status"
[ "$(ar -t a </dev/null | tr '\n' ' ')" = 'f1 g ' ] || pc_fail "ar -d did not match the last component"
pc_pass
