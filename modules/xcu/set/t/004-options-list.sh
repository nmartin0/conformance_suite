# set -o writes the option settings to standard output and set +o writes
# commands that read back into the same option settings.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.004
# MUTANT    set.so_nothing
# MUTANT    set.so_stderr
# MUTANT    set.so_both
# MUTANT    set.so_static
# MUTANT    set.po_nothing
# MUTANT    set.po_stderr
# MUTANT    set.po_both
# MUTANT    set.po_only_on
# MUTANT    set.po_all_off
# MUTANT    set.po_all_on
# MUTANT    set.po_head
# VARIANT   set.so_on_only
# VARIANT   set.so_tab
# VARIANT   set.po_oneline
# VARIANT   set.po_short
. "$PC_CORE/pc.sh"
pc_begin XCU.set.004
nl='
'
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
: >pc.nc1
set -o >pc.o0 2>pc.e0
set -C
set -o >pc.o1 2>pc.e1
set +C
s0=$(set +o 2>pc.e2)
set -C -f
s1=$(set +o 2>pc.e3)
set +C +f
eval "$s1"
echo pc.nc*
if echo x >pc.nc1; then echo c=0; else echo c=nz; fi
eval "$s0"
echo pc.nc*
if echo y >pc.nc1; then echo c=0; else echo c=nz; fi
PCEOF
pc_test
pcrun
for f in pc.o0 pc.o1 pc.e0 pc.e1 pc.e2 pc.e3; do
	[ -f $f ] || pc_fail "the script did not get as far as writing $f (out='$out')"
done
[ -s pc.o1 ] || pc_fail "set -o wrote nothing to standard output while an option was on"
cmp -s pc.o0 pc.o1 && pc_fail "set -o wrote the same with -C on and off"
for f in pc.e0 pc.e1 pc.e2 pc.e3; do
	[ ! -s $f ] || pc_fail "set -o or set +o wrote to standard error: $(cat $f)"
done
[ "$out" = "pc.nc*${nl}c=nz${nl}pc.nc1${nl}c=0" ] || pc_fail "options not restored by the set +o output (out='$out')"
pc_pass
