# verbose output and listings show the operand as given
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.019
# When an operand has a directory part ("sub/g"), "ar -t", "-tv",
# "-pv", "-dv", "-rv" and "-xv" show the name as the operand was
# written, not as the stored last component.
# MUTANT    ar.rv_basename
# MUTANT    ar.t_basename
# MUTANT    ar.tv_basename
# MUTANT    ar.pv_basename
# MUTANT    ar.xv_basename
# MUTANT    ar.dv_basename
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.019
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
mkdir sub other || pc_unresolved "setup: mkdir failed"
printf 'one\n' >f1; printf 'g\n' >sub/g; printf 'h\n' >other/h
ar -rc a f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: cannot build the archive"
pc_test
ar -rv a sub/g >out 2>err </dev/null || pc_fail "ar -rv a sub/g: nonzero status"
[ -f out ] || pc_fail "no output file"
printf 'a - sub/g\n' >exp
cmp -s out exp || pc_fail "ar -rv of a new file wrote '$(cat out)', expected 'a - sub/g'"
printf 'g2\n' >sub/g
ar -rv a sub/g >out 2>err </dev/null || pc_fail "ar -rv a sub/g again: nonzero status"
[ -f out ] || pc_fail "no output file"
printf 'r - sub/g\n' >exp
cmp -s out exp || pc_fail "ar -rv of a replaced file wrote '$(cat out)', expected 'r - sub/g'"
ar -t a sub/g >out 2>err </dev/null || pc_fail "ar -t a sub/g: nonzero status"
[ -f out ] || pc_fail "no output file"
printf 'sub/g\n' >exp
cmp -s out exp || pc_fail "ar -t a sub/g listed '$(cat out)', expected the operand as given"
ar -tv a sub/g >out 2>err </dev/null || pc_fail "ar -tv a sub/g: nonzero status"
[ -f out ] || pc_fail "no output file"
set -- $(cat out)
[ "$#" -eq 8 ] && [ "$8" = sub/g ] || pc_fail "ar -tv a sub/g wrote '$(cat out)', expected the name sub/g as its last field"
ar -pv a sub/g >out 2>err </dev/null || pc_fail "ar -pv a sub/g: nonzero status"
[ -f out ] || pc_fail "no output file"
{ printf '\n<sub/g>\n\n'; cat sub/g; } >exp
cmp -s out exp || pc_fail "ar -pv a sub/g wrote '$(cat out)', expected the header <sub/g>"
rm -f sub/g
ar -xv a sub/g >out 2>err </dev/null || pc_fail "ar -xv a sub/g: nonzero status"
[ -f out ] || pc_fail "no output file"
printf 'x - sub/g\n' >exp
cmp -s out exp || pc_fail "ar -xv a sub/g wrote '$(cat out)', expected 'x - sub/g'"
ar -dv a other/g >out 2>err </dev/null || pc_fail "ar -dv a other/g: nonzero status"
[ -f out ] || pc_fail "no output file"
printf 'd - other/g\n' >exp
cmp -s out exp || pc_fail "ar -dv a other/g wrote '$(cat out)', expected 'd - other/g'"
pc_pass
