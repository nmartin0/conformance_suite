# SIGTERM removes the target being made
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.027
# When make gets SIGTERM while a command of a target runs, it removes the
# target if its modification time changed, unless the target is listed under
# .PRECIOUS (or .PRECIOUS has no prerequisites) or .PHONY, and it ends in error or by the signal.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.027
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
sh -c 'kill -s TERM $$; echo survived' >probe.out 2>/dev/null </dev/null
[ ! -s probe.out ] || pc_unresolved "setup: SIGTERM is ignored in this environment"
cat >mk.plain <<'EOM'
.POSIX:
t: dep
	echo new >t; kill -s TERM $$PPID; sleep 2
EOM
{ cat mk.plain; printf '.PRECIOUS: t\n'; } >mk.precious
{ cat mk.plain; printf '.PHONY: t\n'; } >mk.phony
{ cat mk.plain; printf '.PRECIOUS:\n'; } >mk.precall
: >dep || pc_unresolved "setup: cannot make the prerequisite"
prep() { : >t && touch -t 200001010000 t || pc_unresolved "setup: cannot make the old target"; touch dep; }
pc_test
prep
make -f mk.plain >o 2>err </dev/null; st=$?
[ "$st" -ne 0 ] || pc_fail "make ended with status 0 although it received SIGTERM"
[ -e t ] && pc_fail "the target being made was not removed after SIGTERM"
prep
make -f mk.precious >o 2>err </dev/null; st=$?
[ "$st" -ne 0 ] || pc_fail ".PRECIOUS: make ended with status 0 although it received SIGTERM"
[ -e t ] || pc_fail "a .PRECIOUS target was removed after SIGTERM"
prep
make -f mk.phony >o 2>err </dev/null; st=$?
[ "$st" -ne 0 ] || pc_fail ".PHONY: make ended with status 0 although it received SIGTERM"
[ -e t ] || pc_fail "a .PHONY target was removed after SIGTERM"
prep
make -f mk.precall >o 2>err </dev/null; st=$?
[ "$st" -ne 0 ] || pc_fail ".PRECIOUS with no prerequisites: make ended with status 0 although it received SIGTERM"
[ -e t ] || pc_fail "with .PRECIOUS and no prerequisites a target was removed after SIGTERM"
pc_pass
