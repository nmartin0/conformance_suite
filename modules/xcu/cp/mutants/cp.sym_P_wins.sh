# if -P is among the options given, it takes effect over -H and -L
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	n=$#; hasp=
	for a; do [ "$a" = -P ] && hasp=1; done
	for a; do
		case $a in -H|-L) [ -n "$hasp" ] && continue ;; esac
		set -- "$@" "$a"
	done
	shift "$n"
	command cp "$@"
)
