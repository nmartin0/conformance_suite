# sh -o name and sh +o name act as set -o and set +o: noglob, errexit,
# nounset and pipefail are accepted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh.015
. "$PC_CORE/pc.sh"
pc_begin XCU.sh.015
: >pc.f1
: >pc.f2
pc_test
r=$(${PC_SH:-sh} -o noglob -c 'echo pc.f*' </dev/null)
[ "$r" = 'pc.f*' ] || pc_fail "-o noglob: got '$r'"
r=$(${PC_SH:-sh} -o noglob +o noglob -c 'echo pc.f*' </dev/null)
[ "$r" = 'pc.f1 pc.f2' ] || pc_fail "-o noglob +o noglob: got '$r'"
r=$(${PC_SH:-sh} -o errexit -c 'false; echo no' </dev/null)
[ -z "$r" ] || pc_fail "-o errexit: got '$r'"
r=$(${PC_SH:-sh} -o nounset -c 'echo "$PCU1"; echo no' </dev/null 2>/dev/null)
[ -z "$r" ] || pc_fail "-o nounset: got '$r'"
${PC_SH:-sh} -o pipefail -c 'false | true' </dev/null
rc=$?
[ "$rc" -eq 1 ] || pc_fail "-o pipefail: status $rc, expected 1"
pc_pass
