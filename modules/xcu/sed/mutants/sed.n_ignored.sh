# sed ignores the -n option
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		case $pcm_a in -n) ;; *) set -- "$@" "$pcm_a" ;; esac
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
}
