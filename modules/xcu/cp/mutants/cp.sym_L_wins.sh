# if -L is among the options given, it takes effect over -H and -P
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	n=$#; hasl=
	for a; do [ "$a" = -L ] && hasl=1; done
	for a; do
		case $a in -H|-P) [ -n "$hasl" ] && continue ;; esac
		set -- "$@" "$a"
	done
	shift "$n"
	command cp "$@"
)
