# With set -u, expanding an unset parameter other than @ and * writes a
# message to standard error and the non-interactive shell exits with a non-
# zero status; $@ and $* are exempt.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.013
# MUTANT    set.u_ignored
# MUTANT    set.u_default_on
# MUTANT    set.u_positional_ok
# MUTANT    set.u_braced_ok
# MUTANT    set.u_arith_ok
# MUTANT    set.u_length_ok
# MUTANT    set.u_trim_ok
# MUTANT    set.u_at_unset
# MUTANT    set.u_star_unset
# MUTANT    set.u_null_error
# MUTANT    set.u_no_msg
# MUTANT    set.u_msg_stdout
# MUTANT    set.u_status0
# MUTANT    set.u_continues
# VARIANT   set.u_status2
# VARIANT   set.u_status125
. "$PC_CORE/pc.sh"
pc_begin XCU.set.013
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
# pcdies LABEL: after pcrun of a script whose expansion must fail
pcdies() {
	[ -z "$out" ] || pc_fail "$1: output '$out', expected none"
	[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "$1: status $rc, expected 1 to 125"
	[ -s pc.err ] || pc_fail "$1: no message on standard error"
}
pc_test
pcscript 'echo "[$PCU1]"'
pcrun
[ "$out" = "[]" ] && [ "$rc" -eq 0 ] || pc_fail "without -u: status $rc out '$out'"
pcscript 'set -u' 'echo "x:$PCU1"' 'echo after'
pcrun
pcdies 'simple expansion'
for form in '${PCU1}' '$1' '$(($PCU1 + 1))' '${#PCU1}' '${PCU1%x}'; do
	pcscript 'set -u' "echo \"[$form]\"" 'echo after'
	pcrun
	pcdies "$form"
done
pcscript 'set -u' 'set --' 'echo "[$@][$*]"'
pcrun
[ "$out" = "[][]" ] && [ "$rc" -eq 0 ] || pc_fail "\$@ and \$* with no parameters: status $rc out '$out' (stderr: $(cat pc.err))"
pcscript 'set -u' 'PCU3=' 'echo "[$PCU3]"'
pcrun
[ "$out" = "[]" ] && [ "$rc" -eq 0 ] || pc_fail "a set but null variable: status $rc out '$out' (stderr: $(cat pc.err))"
pc_pass
