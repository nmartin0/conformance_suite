# Every mandatory utility is found under the PATH that getconf PATH gives.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.2-conformance.001
#
# The names are those of the utility modules whose option is BASE, plus the
# names of the colon, dot and test (open bracket) utilities; localedef is
# left out because it belongs to the Locale Creation option.
. "$PC_CORE/pc.sh"
pc_begin XBD.2-conformance.001
LC_ALL=POSIX
export LC_ALL
std=$(getconf PATH 2>/dev/null </dev/null) || pc_unresolved "getconf PATH failed"
[ -n "$std" ] || pc_unresolved "getconf PATH wrote nothing"
pc_test
PATH=$std
export PATH
missing=
for u in alias at awk basename batch bc break cat cd chgrp chmod chown cksum cmp \
	: comm command continue cp crontab csplit cut date dd df diff dirname . du \
	echo ed env eval exec exit expand export expr false file find fold gencat \
	getconf getopts gettext grep hash head iconv id join kill ln locale logger \
	logname lp ls m4 mesg mkdir mkfifo msgfmt mv newgrp ngettext nice nohup od \
	paste patch pathchk pax pr printf ps pwd read readlink readonly realpath \
	renice return rm rmdir sed set sh shift sleep sort split strings stty tabs \
	tail tee test '[' time timeout times touch tput tr trap true tsort tty \
	ulimit umask unalias uname unexpand uniq unset uudecode uuencode wait wc \
	who write xargs; do
	command -v "$u" >/dev/null 2>&1 </dev/null || missing="$missing $u"
done
[ -z "$missing" ] || pc_fail "not found with PATH from getconf PATH:$missing"
pc_pass
