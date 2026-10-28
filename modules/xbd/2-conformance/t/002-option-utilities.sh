# The utilities of each claimed option are found.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.2-conformance.002
. "$PC_CORE/pc.sh"
pc_begin XBD.2-conformance.002
LC_ALL=POSIX
export LC_ALL
std=$(getconf PATH 2>/dev/null </dev/null) || pc_unresolved "getconf PATH failed"
[ -n "$std" ] || pc_unresolved "getconf PATH wrote nothing"
pc_test
PATH=$std
export PATH
claimed=0
missing=
# opt <getconf name> <utility>...
opt() {
	o=$1
	shift
	v=$(getconf "$o" 2>/dev/null </dev/null) || v=$(getconf "_$o" 2>/dev/null </dev/null) || return 0
	case $v in
	'' | *[!0-9]*) return 0 ;;
	esac
	[ "$v" -gt 0 ] || return 0
	claimed=$((claimed + 1))
	for u in "$@"; do
		command -v "$u" >/dev/null 2>&1 </dev/null || missing="$missing $o:$u"
	done
}
opt POSIX2_C_DEV c17 lex yacc
opt POSIX2_FORT_RUN asa
opt POSIX2_LOCALEDEF localedef
opt POSIX2_SW_DEV ar make nm strip
opt POSIX2_UPE bg ex fc fg jobs man more talk vi
opt XOPEN_UUCP uucp uustat uux
[ "$claimed" -gt 0 ] || pc_notinuse "getconf reports none of the options with utilities as supported"
[ -z "$missing" ] || pc_fail "utilities of a supported option not found:$missing"
pc_pass
