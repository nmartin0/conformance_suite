# The long forms set -o allexport, noglob and noclobber act like -a, -f and
# -C, and set +o name turns them off again.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.set.016
. "$PC_CORE/pc.sh"
pc_begin XCU.set.016
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
: >pc.f1
: >pc.f2
printf '%s\n' 'printf "%s,%s\n" "${PCA-u}" "${PCB-u}"' >pc.show || pc_unresolved "write"
pcscript 'set -o allexport' 'PCA=1' 'set +o allexport' 'PCB=2' '${PC_SH:-sh} ./pc.show' 'set -o noglob' 'echo pc.f*' 'set +o noglob' 'echo pc.f*' 'set -o noclobber' 'echo a >pc.nc1' 'echo b >pc.nc1' 'echo "rc=$?"' 'set +o noclobber' 'echo c >pc.nc1' 'echo "rc2=$?"' 
pc_test
pcrun
want="1,u${nl}pc.f*${nl}pc.f1 pc.f2${nl}rc="
case $out in
"$want"[1-9]*"${nl}rc2=0") ;;
*) pc_fail "got '$out'" ;;
esac
pc_pass
