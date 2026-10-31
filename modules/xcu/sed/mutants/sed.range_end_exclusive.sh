# a numeric range stops one line before its second number
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_a=$(printf '%s\n' "$pcm_a" | awk '{ if (match($0, /^[0-9]+,[0-9]+/)) { split(substr($0, 1, RLENGTH), p, ","); print p[1] "," (p[2] - 1) substr($0, RLENGTH + 1) } else print }')
		set -- "$@" "$pcm_a"
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
}
