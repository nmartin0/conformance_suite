# the source after the one that failed is skipped
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	n=$#; i=0; t=
	for a; do t=$a; done
	[ -d "$t" ] || exec command cp "$@"
	rc=0 skip=
	for a; do
		i=$((i + 1)); [ "$i" -eq "$n" ] && break
		if [ -n "$skip" ]; then skip=; continue; fi
		command cp "$a" "$t" || { rc=1; skip=1; }
	done
	exit "$rc"
)
