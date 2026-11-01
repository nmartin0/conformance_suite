# command turns the status of the utility into 1 unless it is 0, 126 or 127
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
	case $pcm_rc in 0 | 126 | 127) ;; *) pcm_rc=1 ;; esac
	return $pcm_rc
}
