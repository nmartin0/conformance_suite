# The long forms set -o allexport, noglob and noclobber act like -a, -f and
# -C, and set +o name turns them off again.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.016
# MUTANT    set.o_allexport_ignored
# MUTANT    set.plus_o_allexport_ignored
# MUTANT    set.o_noglob_ignored
# MUTANT    set.plus_o_noglob_ignored
# MUTANT    set.o_noclobber_ignored
# MUTANT    set.plus_o_noclobber_ignored
# MUTANT    set.o_noglob_is_noclobber
. "$PC_CORE/pc.sh"
pc_begin XCU.set.016
nl='
'
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
LC_ALL=POSIX
export LC_ALL
: >pc.f1
: >pc.f2
cat >pc.show <<'PCEOF' || pc_unresolved "cannot write pc.show"
printf '%s\n' "${PCA-u},${PCB-u}"
PCEOF
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
set -o allexport
PCA=1
set +o allexport
PCB=2
${PC_SH:-sh} ./pc.show
set -o noglob
echo pc.f*
set +o noglob
echo pc.f*
set -o noclobber
echo a >pc.nc1
if echo b >pc.nc1; then echo c=0; else echo c=nz; fi
set +o noclobber
echo c >pc.nc1
echo "rc2=$?"
PCEOF
pc_test
pcrun
want="1,u${nl}pc.f*${nl}pc.f1 pc.f2${nl}c=nz${nl}rc2=0"
[ "$out" = "$want" ] || pc_fail "got '$out'"
pc_pass
