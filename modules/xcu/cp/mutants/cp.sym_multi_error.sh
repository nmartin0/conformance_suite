# more than one of -H -L -P is refused
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() (
	c=0
	for a; do case $a in -H|-L|-P) c=$((c + 1)) ;; esac; done
	if [ "$c" -gt 1 ]; then echo "cp: conflicting options" >&2; exit 1; fi
	command cp "$@"
)
