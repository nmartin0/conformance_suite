# a relative name with a slash is written as the working directory joined with it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The shell function stands in for the utility; helpers of the test itself
# (brackets, printf) pass through to the real one.
pcm_real() { ( unset -f command; command "$@" ); }
command() {
	case $1 in '[' | printf) pcm_real "$@"; return ;; esac
	case $1 in
	-v)
		pcm_out=$(pcm_real "$@"); pcm_rc=$?
		if [ "$pcm_rc" -eq 0 ] && case $pcm_out in /*) case $2 in */*) true ;; *) false ;; esac ;; *) false ;; esac; then
			case $2 in /*) printf '%s\n' "$2" ;; *) printf '%s/%s\n' "$PWD" "$2" ;; esac
			return $pcm_rc
		fi
		if [ -n "$pcm_out" ]; then printf '%s\n' "$pcm_out"; fi
		return $pcm_rc ;;
	esac
	pcm_real "$@"
}
