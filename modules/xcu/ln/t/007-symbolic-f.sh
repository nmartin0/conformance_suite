# ln -s fails on an existing destination without -f, and replaces it with -f.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ln.007
. "$PC_CORE/pc.sh"
pc_begin XCU.ln.007
linkof() {
	lk=$(ls -ld "$1") || return 1
	lk=${lk##* -> }
}

ln -s x lnk || pc_fail "cannot create the first link"
pc_test
ln -s y lnk >out 2>err </dev/null
rc=$?
[ "$rc" -ne 0 ] || pc_fail "ln -s exited 0 although lnk exists"
linkof lnk || pc_fail "cannot list lnk"
[ "$lk" = x ] || pc_fail "lnk was changed to '$lk'"
ln -s -f y lnk >out 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ln -s -f exited with status $rc"
linkof lnk || pc_fail "cannot list lnk"
[ "$lk" = y ] || pc_fail "lnk contains '$lk', expected y"
pc_pass
