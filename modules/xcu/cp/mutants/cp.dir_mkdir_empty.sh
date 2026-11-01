# a directory source makes an empty directory in the target
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	n=$#; i=0; t=
	for a; do t=$a; done
	[ -d "$t" ] || exec command cp "$@"
	rc=0
	for a; do
		i=$((i + 1)); [ "$i" -eq "$n" ] && break
		if [ -d "$a" ]; then mkdir "$t/${a##*/}"; echo "cp: $a: is a directory" >&2; rc=1; continue; fi
		command cp "$a" "$t" || rc=1
	done
	exit "$rc"
)
