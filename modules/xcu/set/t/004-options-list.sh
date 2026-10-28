# set -o writes the option settings to standard output and set +o writes
# commands that read back into the same option settings.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.004
. "$PC_CORE/pc.sh"
pc_begin XCU.set.004
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
pcscript ': >pc.nc1' 'set -C -f' 's=$(set +o)' 'set +C +f' 'eval "$s"' 'echo pc.nc*' 'echo x >pc.nc1' 'echo "rc=$?"' 'set -o >pc.so' 
pc_test
pcrun
[ -s pc.so ] || pc_fail "set -o wrote nothing"
case $out in
"pc.nc*${nl}rc="[1-9]*) ;;
*) pc_fail "options not restored by the set +o output (out='$out')" ;;
esac
pc_pass
