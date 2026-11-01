# command -v for a function exits 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The shell function stands in for the utility; helpers of the test itself
# (brackets, printf) pass through to the real one.
pcm_real() { ( unset -f command; command "$@" ); }
pcm_isspecial() {
	case $1 in break | : | continue | . | eval | exec | exit | export | readonly | return | set | shift | times | trap | unset) return 0 ;; esac
	return 1
}
pcm_isreserved() {
	case $1 in '!' | '{' | '}' | case | do | done | elif | else | esac | fi | for | if | in | then | until | while) return 0 ;; esac
	return 1
}
command() {
	case $1 in '[' | printf) pcm_real "$@"; return ;; esac
	case $1 in
	-v)
		pcm_out=$(pcm_real "$@"); pcm_rc=$?
		if [ "$pcm_rc" -eq 0 ] && ! pcm_isspecial "$2" && ! pcm_isreserved "$2" && [ "$pcm_out" = "$2" ]; then
			printf '%s\n' "$2"
			pcm_rc=1
			return $pcm_rc
		fi
		if [ -n "$pcm_out" ]; then printf '%s\n' "$pcm_out"; fi
		return $pcm_rc ;;
	esac
	pcm_real "$@"
}
