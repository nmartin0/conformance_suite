# of several of -H -L -P the first one given takes effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	n=$#; seen=
	for a; do
		case $a in -H|-L|-P) [ -n "$seen" ] && continue; seen=1 ;; esac
		set -- "$@" "$a"
	done
	shift "$n"
	command cp "$@"
)
