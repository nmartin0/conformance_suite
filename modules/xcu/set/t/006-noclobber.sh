# With set -C, > does not overwrite an existing regular file (the command
# fails) but >| does, and a file that does not exist yet or a non-regular
# file such as /dev/null can still be the target of >.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.006
# MUTANT    set.c_ignored
# MUTANT    set.c_default_on
# MUTANT    set.c_blocks_new
# MUTANT    set.c_blocks_devnull
# MUTANT    set.c_clobber_op_ignored
# MUTANT    set.c_status0
# MUTANT    set.c_no_diag
# MUTANT    set.c_truncates
# MUTANT    set.c_symlink_unchecked
. "$PC_CORE/pc.sh"
pc_begin XCU.set.006
nl='
'
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
ln -s pc.nc1 pc.ln 2>/dev/null || pc_unresolved "cannot make a symbolic link"
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
echo a >pc.nc0
echo b >pc.nc0
echo "d=$?"
set -C
echo one >pc.nc1
echo "n=$?"
if echo two >pc.nc1; then echo e=0; else echo e=nz; fi
cat pc.nc1
echo three >|pc.nc1
echo "o=$?"
cat pc.nc1
echo four >/dev/null
echo "z=$?"
if echo x >pc.ln; then echo l=0; else echo l=nz; fi
cat pc.nc1
PCEOF
pc_test
pcrun
want="d=0${nl}n=0${nl}e=nz${nl}one${nl}o=0${nl}three${nl}z=0${nl}l=nz${nl}three"
[ "$out" = "$want" ] || pc_fail "got '$out'"
[ -s pc.err ] || pc_fail "no diagnostic on standard error for the refused redirection"
[ "$(cat pc.nc0)" = b ] || pc_fail "without -C the second > did not replace the file"
pc_pass
