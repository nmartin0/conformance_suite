# command turns every non-zero status into 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The shell function stands in for the utility; helpers of the test itself
# (brackets, printf) pass through to the real one.
pcm_real() { ( unset -f command; command "$@" ); }
command() {
	case $1 in '[' | printf) pcm_real "$@"; return ;; esac
	case $1 in
	-*) pcm_real "$@"; return ;;
	esac
	pcm_real "$@"; pcm_rc=$?
	if [ "$pcm_rc" -ne 0 ]; then pcm_rc=1; fi
	return $pcm_rc
}
