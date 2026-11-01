# An unknown condition name makes trap write a warning to standard error and
# return non-zero, but the shell does not exit.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.trap.013
# MUTANT    trap.invalid_silent
# MUTANT    trap.invalid_status_zero
# MUTANT    trap.invalid_exits
# MUTANT    trap.invalid_to_stdout
# MUTANT    trap.invalid_reset_accepted
# MUTANT    trap.invalid_ignore_accepted
# VARIANT   trap.invalid_status_big
. "$PC_CORE/pc.sh"
pc_begin XCU.trap.013
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
pc_test
for form in '"echo x"' '-' '""'; do
	pcscript "trap $form PCNOSUCHSIG_XYZ" 'echo "rc=$?"' 'echo after'
	pcrun
	case $out in
	"rc="[1-9]*"${nl}after") ;;
	*) pc_fail "trap $form: got '$out', expected a non-zero rc line and after" ;;
	esac
	[ -s pc.err ] || pc_fail "trap $form: no warning on standard error"
done
pc_pass
