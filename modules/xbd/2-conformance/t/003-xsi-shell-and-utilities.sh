# An XSI system supports the User Portability and Terminal Characteristics options, locale creation and c17.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.2-conformance.003
. "$PC_CORE/pc.sh"
pc_begin XBD.2-conformance.003
LC_ALL=POSIX
export LC_ALL
std=$(getconf PATH 2>/dev/null </dev/null) || pc_unresolved "getconf PATH failed"
[ -n "$std" ] || pc_unresolved "getconf PATH wrote nothing"
pc_test
for o in POSIX2_UPE POSIX2_CHAR_TERM POSIX2_LOCALEDEF; do
	v=$(getconf "$o" 2>/dev/null </dev/null) || pc_fail "getconf $o failed on an XSI system"
	case $v in
	undefined | -1 | '') pc_fail "getconf $o wrote '$v' on an XSI system" ;;
	esac
done
PATH=$std
export PATH
command -v c17 >/dev/null 2>&1 </dev/null || pc_fail "c17 was not found on an XSI system"
pc_pass
