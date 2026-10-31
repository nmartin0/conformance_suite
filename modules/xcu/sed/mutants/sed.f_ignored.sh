# the -f option and its script file are ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_n=$((pcm_n - 1))
		case $pcm_a in
		-f) shift; pcm_n=$((pcm_n - 1)) ;;
		*) set -- "$@" "$pcm_a" ;;
		esac
	done
	command sed "$@"
}
