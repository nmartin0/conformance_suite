# only nine different w files are supported; the tenth is not written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_a=$(printf '%s\n' "$pcm_a" | awk '/^w[ \t]/ { c++; if (c > 9) { print "w /dev/null"; next } } { print }')
		set -- "$@" "$pcm_a"
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
}
