# With set -a, every variable assignment (plain, by an expansion, by read)
# gives the variable the export attribute; set +a ends that.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.005
. "$PC_CORE/pc.sh"
pc_begin XCU.set.005
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
printf '%s\n' line >pc.rin || pc_unresolved "write"
printf '%s\n' 'printf "%s,%s,%s,%s\n" "${PCA-u}" "${PCB-u}" "${PCC-u}" "${PCD-u}"' >pc.show || pc_unresolved "write"
pcscript 'set -a' 'PCA=1' 'read PCB <pc.rin' ': ${PCC:=3}' 'set +a' 'PCD=4' '${PC_SH:-sh} ./pc.show' 
pc_test
pcrun
[ "$out" = "1,line,3,u" ] || pc_fail "got '$out', expected 1,line,3,u"
pc_pass
