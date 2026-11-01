# command swaps the first two arguments of the utility
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The shell function stands in for the utility; helpers of the test itself
# (brackets, printf) pass through to the real one.
pcm_real() { ( unset -f command; command "$@" ); }
command() {
	case $1 in '[' | printf) pcm_real "$@"; return ;; esac
	case $1 in
	-*) pcm_real "$@"; return ;;
	esac
	if [ $# -ge 3 ]; then
		pcm_n=$1; pcm_a=$2; pcm_b=$3; shift 3
		set -- "$pcm_n" "$pcm_b" "$pcm_a" "$@"
	fi
	pcm_real "$@"
}
