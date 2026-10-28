# Options given on the command line act as the same options of set,
# including the plus form that reverses one: -a, -C, -e, -f, -u (and -f
# followed by +f).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.012
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.012
: >pc.f1
: >pc.f2
printf '%s\n' 'printf "%s\n" "${PCA-u}"' >pc.show || pc_unresolved "write"
pc_test
r=$(${PC_SH:-sh} -a -c 'PCA=1; ${PC_SH:-sh} ./pc.show' </dev/null)
[ "$r" = 1 ] || pc_fail "-a: got '$r'"
r=$(${PC_SH:-sh} -C -c 'echo a >pc.nc1; echo b >pc.nc1; echo "rc=$?"' </dev/null 2>/dev/null)
case $r in
"rc="[1-9]*) ;;
*) pc_fail "-C: got '$r'" ;;
esac
r=$(${PC_SH:-sh} -e -c 'false; echo no' </dev/null)
[ -z "$r" ] || pc_fail "-e: got '$r'"
r=$(${PC_SH:-sh} -f -c 'echo pc.f*' </dev/null)
[ "$r" = 'pc.f*' ] || pc_fail "-f: got '$r'"
r=$(${PC_SH:-sh} -f +f -c 'echo pc.f*' </dev/null)
[ "$r" = 'pc.f1 pc.f2' ] || pc_fail "-f +f: got '$r'"
r=$(${PC_SH:-sh} -u -c 'echo "$PCU1"; echo after' </dev/null 2>/dev/null)
[ -z "$r" ] || pc_fail "-u: got '$r'"
${PC_SH:-sh} -u -c 'echo "$PCU1"' </dev/null 2>/dev/null
rc=$?
[ "$rc" -ge 1 ] && [ "$rc" -le 125 ] || pc_fail "-u: status $rc, expected 1 to 125"
pc_pass
