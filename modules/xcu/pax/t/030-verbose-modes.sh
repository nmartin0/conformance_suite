# pax -v in read and copy mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.030
# "-v" in read mode and in copy mode writes each member or file name as one
# line to standard error and nothing to standard output; with -s the name
# written is the substituted one (also in write mode).
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.030
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '1\n' >f1 && printf '2\n' >f2 || pc_unresolved "setup: cannot write files"
pax -w f1 f2 >a 2>/dev/null </dev/null || pc_unresolved "setup: pax -w failed"
mkdir dest || pc_unresolved "setup: mkdir failed"
rm -f f1 f2
printf '1\n' >f1 && printf '2\n' >f2 || pc_unresolved "setup: cannot write files"
pc_test
pax -r -v <a >o 2>e
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pax -r -v: exit status $rc"
[ -s o ] && pc_fail "pax -r -v wrote to standard output: $(cat o)"
grep -x f1 e >/dev/null || pc_fail "pax -r -v did not write the line f1 to standard error"
grep -x f2 e >/dev/null || pc_fail "pax -r -v did not write the line f2 to standard error"
rm -f f1 f2
pax -r -v -s ',f,g,' <a >o 2>e || pc_fail "pax -r -v -s: nonzero status"
grep -x g1 e >/dev/null || pc_fail "pax -r -v -s ',f,g,' did not write the substituted name g1"
grep -x g2 e >/dev/null || pc_fail "pax -r -v -s ',f,g,' did not write the substituted name g2"
grep -x f1 e >/dev/null && pc_fail "pax -r -v -s wrote the original name f1 instead of the substituted one"
printf '1\n' >f1 && printf '2\n' >f2 || pc_unresolved "setup: cannot write files"
pax -rw -v f1 f2 dest >o 2>e </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "pax -rw -v: exit status $rc"
[ -s o ] && pc_fail "pax -rw -v wrote to standard output: $(cat o)"
grep -x f1 e >/dev/null || pc_fail "pax -rw -v did not write the line f1 to standard error"
grep -x f2 e >/dev/null || pc_fail "pax -rw -v did not write the line f2 to standard error"
pax -w -v -s ',f,g,' f1 f2 >a.s 2>e </dev/null || pc_fail "pax -w -v -s: nonzero status"
grep -x g1 e >/dev/null || pc_fail "pax -w -v -s ',f,g,' did not write the substituted name g1"
grep -x f1 e >/dev/null && pc_fail "pax -w -v -s wrote the original name f1 instead of the substituted one"
pc_pass
