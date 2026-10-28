# With job control on (set -m), an asynchronous list is a background job
# whose process is a process group leader: its process group ID equals its
# process ID.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-jobs.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-jobs.001
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
command -v ps >/dev/null || pc_unresolved "ps is not available"
pcscript 'set -m' 'sleep 5 &' 'p=$!' 'g=$(ps -o pgid= -p "$p" | tr -d " ")' 'kill "$p"' 'wait' 'echo "$p $g"' 
pc_test
pcrun
p=${out%% *}
g=${out#* }
[ -n "$p" ] && [ -n "$g" ] || pc_unresolved "could not read the process group (out='$out', stderr: $(cat pc.err))"
[ "$p" = "$g" ] || pc_fail "process id $p, process group $g"
pc_pass
