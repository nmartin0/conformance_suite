# export -p writes one line per exported variable as export name=value
# (value quoted so the line can be read back), and just export name for an
# exported variable that is unset; a variable that is not exported is not
# listed.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.export.004
. "$PC_CORE/pc.sh"
pc_begin XCU.export.004
PCEA="x'y\"z\$w\\v"
export PCEA
export PCEB='a b'
unset PCEU
export PCEU
PCNX=notexported
pc_test
export -p >pc.exp
grep '^export PCEA=' pc.exp >pc.line1 || pc_fail "no 'export PCEA=' line"
grep '^export PCEB=' pc.exp >pc.line2 || pc_fail "no 'export PCEB=' line"
grep '^export PCEU$' pc.exp >/dev/null || pc_fail "no 'export PCEU' line for the unset exported name"
grep 'PCNX' pc.exp >/dev/null && pc_fail "a variable that is not exported was listed"
want="x'y\"z\$w\\v"
r=$(${PC_SH:-sh} -c '. ./pc.line1; printf %s "$PCEA"')
[ "$r" = "$want" ] || pc_fail "reading the PCEA line back gave '$r', expected '$want'"
r=$(${PC_SH:-sh} -c '. ./pc.line2; printf %s "$PCEB"')
[ "$r" = "a b" ] || pc_fail "reading the PCEB line back gave '$r', expected 'a b'"
pc_pass
