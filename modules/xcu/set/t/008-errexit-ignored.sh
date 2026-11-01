# set -e does not end the shell for a failure in an if, elif, while or until
# condition, in a command preceded by !, or in a command of an AND-OR list
# other than the last.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.008
# MUTANT    set.e_if_cond
# MUTANT    set.e_elif_cond
# MUTANT    set.e_while_cond
# MUTANT    set.e_until_cond
# MUTANT    set.e_not
# MUTANT    set.e_and_nonlast
# MUTANT    set.e_or_nonlast
# MUTANT    set.e_cond_list_first
# MUTANT    set.e_fn_in_cond
# MUTANT    set.e_fn_in_andor
# MUTANT    set.e_group_in_andor
# MUTANT    set.e_and_last_ignored
# MUTANT    set.e_or_last_ignored
# MUTANT    set.e_if_body_ignored
# MUTANT    set.e_while_body_ignored
. "$PC_CORE/pc.sh"
pc_begin XCU.set.008
nl='
'
# pcscript: write the arguments, one per line, to pc.s
pcscript() {
	printf '%s\n' "$@" >pc.s || pc_unresolved "cannot write pc.s"
}
# pcrun: run pc.s in a fresh shell; out = its standard output, rc = its
# exit status, its standard error is left in pc.err.
pcrun() {
	out=$(${PC_SH:-sh} pc.s 2>pc.err </dev/null)
	rc=$?
}
# pcexp LABEL OUT RC: run pc.s; it must write OUT and exit with RC
pcexp() {
	pcrun
	[ "$out" = "$2" ] && [ "$rc" -eq "$3" ] || pc_fail "$1: status $rc and output '$out', expected status $3 and output '$2'"
}
cat >pc.s <<'PCEOF' || pc_unresolved "cannot write pc.s"
set -e
pcf_if() { return 1; }
pcf_elif() { return 1; }
pcf_while() { return 1; }
pcf_until() { [ -e pc.u ] && return 0; : >pc.u; return 1; }
pcf_not() { return 1; }
pcf_and() { return 1; }
pcf_or() { return 1; }
pcf_lf() { return 1; }
pcf_body() { false; echo reached; }
pcf_body2() { false; echo reached2; }
if pcf_if; then echo bad; fi
echo m1
if false; then :; elif pcf_elif; then :; fi
echo m2
while pcf_while; do :; done
echo m3
until pcf_until; do :; done
echo m4
! pcf_not
echo m5
pcf_and && echo bad
echo m6
pcf_or || true
echo m7
if pcf_lf; true; then echo yes; fi
echo m8
if pcf_body; then echo m9; fi
pcf_body2 || echo bad
echo m10
{ false; echo reached3; } || echo bad
echo m11
PCEOF
pc_test
want="m1${nl}m2${nl}m3${nl}m4${nl}m5${nl}m6${nl}m7${nl}yes${nl}m8${nl}reached${nl}m9${nl}reached2${nl}m10${nl}reached3${nl}m11"
pcexp "failures in contexts that ignore -e" "$want" 0
pcscript 'set -e' 'true && false' 'echo after'
pcexp "last command of an AND list" "" 1
pcscript 'set -e' 'false || false' 'echo after'
pcexp "last command of an OR list" "" 1
pcscript 'set -e' 'if true; then false; echo after; fi' 'echo after2'
pcexp "body of an if" "" 1
pcscript 'set -e' 'pcn=0' 'while [ "$pcn" -lt 1 ]; do pcn=1; false; echo after; done' 'echo after2'
pcexp "body of a while" "" 1
pc_pass
