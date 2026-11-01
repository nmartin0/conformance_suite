# the E in a group such as -nE is lost
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		case $pcm_a in -nE) pcm_a=-n ;; esac
		set -- "$@" "$pcm_a"
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
}
