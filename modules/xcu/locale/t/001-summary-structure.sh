# locale with no operands: structure of the summary.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.locale.001
. "$PC_CORE/pc.sh"
pc_begin XCU.locale.001
pc_test
check() { # check LABEL FIRST (env words after)
	label=$1 first=$2
	shift 2
	env -i PATH="$PATH" "$@" locale >out 2>err </dev/null
	rc=$?
	[ "$rc" -eq 0 ] || pc_fail "$label: locale exited with status $rc"
	[ -s err ] && pc_fail "$label: locale wrote to standard error"
	n=0 last= got=
	for c in LC_CTYPE LC_COLLATE LC_MONETARY LC_NUMERIC LC_TIME LC_MESSAGES; do
		eval "seen_$c=no"
	done
	while IFS= read -r l; do
		n=$((n + 1))
		last=$l
		[ "$n" -eq 1 ] && got=$l
		for c in LC_CTYPE LC_COLLATE LC_MONETARY LC_NUMERIC LC_TIME LC_MESSAGES; do
			case $l in "$c="*) eval "seen_$c=yes" ;; esac
		done
	done <out
	[ "$got" = "$first" ] || pc_fail "$label: first line is '$got', expected '$first'"
	case $last in
	LC_ALL=*) ;;
	*) pc_fail "$label: last line is '$last', expected a line LC_ALL=" ;;
	esac
	for c in LC_CTYPE LC_COLLATE LC_MONETARY LC_NUMERIC LC_TIME LC_MESSAGES; do
		eval "s=\$seen_$c"
		[ "$s" = yes ] || pc_fail "$label: no line for $c"
	done
}
check "nothing set" "LANG="
check "LANG set" "LANG=C" LANG=C
pc_pass
