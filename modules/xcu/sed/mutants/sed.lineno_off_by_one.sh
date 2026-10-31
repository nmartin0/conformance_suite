# a leading line number address selects the following line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_a=$(printf '%s\n' "$pcm_a" | awk '{ if (match($0, /^[0-9]+/)) print (substr($0, 1, RLENGTH) + 1) substr($0, RLENGTH + 1); else print }')
		set -- "$@" "$pcm_a"
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
}
