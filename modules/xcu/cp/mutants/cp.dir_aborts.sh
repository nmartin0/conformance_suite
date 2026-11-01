# the first directory source ends the run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	n=$#; i=0; t=
	for a; do t=$a; done
	[ -d "$t" ] || exec command cp "$@"
	rc=0
	for a; do
		i=$((i + 1)); [ "$i" -eq "$n" ] && break
		if [ -d "$a" ]; then echo "cp: $a: is a directory" >&2; exit 1; fi
		command cp "$a" "$t" || rc=1
	done
	exit "$rc"
)
