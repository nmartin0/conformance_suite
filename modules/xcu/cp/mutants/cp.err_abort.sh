# the first source that cannot be copied ends the run
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	n=$#; i=0; t=
	for a; do t=$a; done
	[ -d "$t" ] || exec command cp "$@"
	rc=0
	for a; do
		i=$((i + 1)); [ "$i" -eq "$n" ] && break
		command cp "$a" "$t" || { rc=1; break; }
	done
	exit "$rc"
)
