# Set variables unquoted, the others quoted.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.locale.002
. "$PC_CORE/pc.sh"
pc_begin XCU.locale.002
pc_test
env -i PATH="$PATH" LANG=C LC_TIME=POSIX LC_NUMERIC=C locale >out 2>err </dev/null || pc_fail "locale failed"
t=no n=no
while IFS= read -r l; do
	case $l in
	LC_TIME=POSIX) t=yes ;;
	LC_NUMERIC=C) n=yes ;;
	LC_TIME=* | LC_NUMERIC=*) pc_fail "set variable written as '$l'" ;;
	LC_CTYPE=* | LC_COLLATE=* | LC_MONETARY=* | LC_MESSAGES=*)
		case $l in
		LC_CTYPE=\"C\" | LC_CTYPE=\"POSIX\") ;;
		LC_COLLATE=\"C\" | LC_COLLATE=\"POSIX\") ;;
		LC_MONETARY=\"C\" | LC_MONETARY=\"POSIX\") ;;
		LC_MESSAGES=\"C\" | LC_MESSAGES=\"POSIX\") ;;
		*) pc_fail "variable not set in the environment written as '$l', expected a quoted name of the POSIX locale" ;;
		esac
		;;
	esac
done <out
[ "$t" = yes ] || pc_fail "no line LC_TIME=POSIX"
[ "$n" = yes ] || pc_fail "no line LC_NUMERIC=C"
pc_pass
