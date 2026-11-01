# command drops empty arguments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The shell function stands in for the utility; helpers of the test itself
# (brackets, printf) pass through to the real one.
pcm_real() { ( unset -f command; command "$@" ); }
command() {
	case $1 in '[' | printf) pcm_real "$@"; return ;; esac
	case $1 in
	-*) pcm_real "$@"; return ;;
	esac
	pcm_cnt=$#
	while [ "$pcm_cnt" -gt 0 ]; do
		pcm_x=$1; shift
		if [ -n "$pcm_x" ]; then set -- "$@" "$pcm_x"; fi
		pcm_cnt=$((pcm_cnt - 1))
	done
	pcm_real "$@"
}
