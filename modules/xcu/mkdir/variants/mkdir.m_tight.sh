# a conforming mkdir that, for an octal -m mode, creates the directory with no
# permissions at all and then sets the requested mode
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because, with -m, the mode handed to the creation call is
# unspecified as long as the directory is never less restrictive than the
# requested mode (XCU mkdir DESCRIPTION); no permissions is more restrictive.
mkdir() (
	popt= mopt=
	OPTIND=1
	while getopts pm: o; do
		case $o in
		p) popt=1 ;;
		m) mopt=$OPTARG ;;
		*) exit 2 ;;
		esac
	done
	shift $((OPTIND - 1))
	case $mopt in
	[0-7]*)
		rc=0
		for d; do
			if [ -d "$d" ]; then had=1; else had=; fi
			if [ -n "$popt" ]; then
				command mkdir -p -m 000 "$d" || { rc=1; continue; }
			else
				command mkdir -m 000 "$d" || { rc=1; continue; }
			fi
			[ -n "$had" ] || chmod "$mopt" "$d" || rc=1
		done
		exit $rc
		;;
	esac
	if [ -n "$popt" ]; then
		if [ -n "$mopt" ]; then command mkdir -p -m "$mopt" "$@"; else command mkdir -p "$@"; fi
	else
		if [ -n "$mopt" ]; then command mkdir -m "$mopt" "$@"; else command mkdir "$@"; fi
	fi
)
