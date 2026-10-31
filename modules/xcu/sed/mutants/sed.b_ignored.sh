# b does not branch
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pcm_nl='
'
pcm_map() {
	pcm_x=$1
	shift
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_a=$(printf '%s\n' "$pcm_a" | command sed -E "$pcm_x")
		set -- "$@" "$pcm_a"
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
}
sed() { pcm_map 's#^(/x/)?b[[:blank:]]*[a-z]*$#\1y/a/a/#' "$@"; }
