# set with no options or operands lists the shell variables as name=value in
# the collation order of the current locale, with values quoted so that the
# lines can be read back.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.003
# MUTANT    set.list_reverse
# MUTANT    set.list_casefold
# MUTANT    set.list_unquoted
# MUTANT    set.list_naive_sq
# MUTANT    set.list_naive_dq
# MUTANT    set.list_exported
# MUTANT    set.list_stderr
# MUTANT    set.list_spaced
# MUTANT    set.list_nothing
# VARIANT   set.list_sq
# VARIANT   set.list_dq
# VARIANT   set.list_bare
. "$PC_CORE/pc.sh"
pc_begin XCU.set.003
nl='
'
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
LC_ALL=POSIX
export LC_ALL
PCSET_B='second one'
PCSET_A="it's \"q\" \$x \\ back"
PCSET_Z=zed
PCSET_a=lower
set >pc.lst
PCEOF
pc_test
pcrun
[ -f pc.lst ] || pc_fail "set wrote no listing to standard output"
[ ! -s pc.err ] || pc_fail "set wrote to standard error: $(cat pc.err)"
grep '^PCSET_' pc.lst >pc.re
names=$(sed -n 's/^\(PCSET_[A-Za-z]\)=.*/\1/p' pc.re | tr '\n' ' ')
[ "$names" = "PCSET_A PCSET_B PCSET_Z PCSET_a " ] || pc_fail "listed in the order '$names', expected PCSET_A PCSET_B PCSET_Z PCSET_a"
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
PCSET_A=x
PCSET_B=x
PCSET_Z=x
PCSET_a=x
. ./pc.re
printf '%s\n' "$PCSET_A" "$PCSET_B" "$PCSET_Z" "$PCSET_a"
PCEOF
pcrun
want="it's \"q\" \$x \\ back${nl}second one${nl}zed${nl}lower"
[ "$out" = "$want" ] || pc_fail "reading the lines back gave '$out' (stderr: $(cat pc.err))"
pc_pass
