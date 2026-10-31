# the status is that of the last source only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	n=$#; i=0; t=
	for a; do t=$a; done
	[ -d "$t" ] || exec command cp "$@"
	rc=0
	for a; do
		i=$((i + 1)); [ "$i" -eq "$n" ] && break
		command cp "$a" "$t"; rc=$?
	done
	exit "$rc"
)
