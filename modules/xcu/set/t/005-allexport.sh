# With set -a, every variable assignment (plain, by an expansion, by read)
# gives the variable the export attribute; set +a ends that.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.005
# MUTANT    set.a_ignored
# MUTANT    set.a_default_on
# MUTANT    set.plus_a_ignored
# MUTANT    set.a_not_plain
# MUTANT    set.a_not_existing
# MUTANT    set.a_not_read
# MUTANT    set.a_not_expansion
# MUTANT    set.a_not_arith
# MUTANT    set.a_not_getopts
# MUTANT    set.a_not_cd
. "$PC_CORE/pc.sh"
pc_begin XCU.set.005
nl='
'
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
printf '%s\n' line >pc.rin || pc_unresolved "write"
cat >pc.show <<'PCEOF' || pc_unresolved "cannot write pc.show"
printf '%s\n' "${PCZ-u} ${PCA-u} ${PCB-u} ${PCC-u} ${PCEI-u} ${PCG-u} ${PCX0-u} ${OLDPWD+S} ${PCD-u}"
PCEOF
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
PCW=$PWD
PCZ=0
PCX0=old
set -a
PCA=1
read PCB <pc.rin
: ${PCC:=3}
: $((PCEI = 5))
PCX0=new
set -- -a
getopts a PCG "$@"
unset OLDPWD
cd /
set +a
PCD=4
${PC_SH:-sh} "$PCW/pc.show"
PCEOF
pc_test
pcrun
[ "$out" = "u 1 line 3 5 a new S u" ] || pc_fail "got '$out', expected 'u 1 line 3 5 a new S u' (stderr: $(cat pc.err))"
pc_pass
