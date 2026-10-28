# nl restarts the numbering at each logical page unless -p is given; header and footer lines are not numbered by default.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.007
. "$PC_CORE/pc.sh"
pc_begin XCU.nl.007
LC_ALL=POSIX
export LC_ALL
NL='
'
numbered() {
	while IFS= read -r l; do
		case $l in
		*"$1"*) printf '%s\n' "$l" ;;
		esac
	done <"$2"
}
printf '%s\n' '\:\:\:' H1 '\:\:' B1 B2 '\:' F1 '\:\:\:' H2 '\:\:' B3 '\:' F2 >in || pc_fail "cannot create input"
pc_test
nl -s '|' in >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nl gave status $rc"
got=$(numbered "|" out; printf x)
[ "$got" = "     1|B1${NL}     2|B2${NL}     1|B3${NL}x" ] || pc_fail "numbering was not restarted at the second page, or header/footer lines were numbered"
nl -p -s '|' in >out 2>/dev/null </dev/null
got=$(numbered "|" out; printf x)
[ "$got" = "     1|B1${NL}     2|B2${NL}     3|B3${NL}x" ] || pc_fail "-p did not keep the numbering across pages"
pc_pass
