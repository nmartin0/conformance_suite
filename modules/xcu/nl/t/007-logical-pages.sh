# nl restarts the numbering at each logical page unless -p is given; header and footer lines are not numbered by default.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.nl.007
# MUTANT    nl.hdr_numbered
# MUTANT    nl.ftr_numbered
# MUTANT    nl.page_no_reset
# MUTANT    nl.p_ignored
# MUTANT    nl.reset_to_1
# MUTANT    nl.delim_prefix
# MUTANT    nl.hdr_delim_as_body
# MUTANT    nl.ftr_delim_ignored
# MUTANT    nl.body_delim_ignored
# MUTANT    nl.b_sets_all
# VARIANT   nl.delim_dropped
# VARIANT   nl.delim_echo
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
D1='\:'
printf '%s\n' '\:\:\:' H1 '\:\:' B1 B2 '\:\:x' '\:' F1 '\:\:\:' H2 '\:\:' B3 '\:' F2 >in || pc_fail "cannot create input"
pc_test
nl -s '|' in >out 2>/dev/null </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "nl gave status $rc"
[ -f out ] || pc_fail "no output was captured"
got=$(numbered "|" out; printf x)
[ "$got" = "     1|B1${NL}     2|B2${NL}     3|${D1}${D1}x${NL}     1|B3${NL}x" ] || pc_fail "numbering was not restarted at the second page, header/footer lines were numbered, or a line that only begins with the delimiter characters was taken as a delimiter"
nl -p -s '|' in >out 2>/dev/null </dev/null
got=$(numbered "|" out; printf x)
[ "$got" = "     1|B1${NL}     2|B2${NL}     3|${D1}${D1}x${NL}     4|B3${NL}x" ] || pc_fail "-p did not keep the numbering across pages"
nl -v 5 -i 2 -s '|' in >out 2>/dev/null </dev/null
got=$(numbered "|" out; printf x)
[ "$got" = "     5|B1${NL}     7|B2${NL}     9|${D1}${D1}x${NL}     5|B3${NL}x" ] || pc_fail "the numbering did not restart at the start number"
nl -b a -s '|' in >out 2>/dev/null </dev/null
got=$(numbered "|" out; printf x)
[ "$got" = "     1|B1${NL}     2|B2${NL}     3|${D1}${D1}x${NL}     1|B3${NL}x" ] || pc_fail "-b a changed the numbering of header or footer lines"
pc_pass
