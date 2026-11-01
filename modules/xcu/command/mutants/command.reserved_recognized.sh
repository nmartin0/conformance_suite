# command treats a reserved word as a syntax error
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
	-*) pcm_real "$@"; return ;;
	esac
	if pcm_isreserved "$1"; then
		printf 'command: syntax error near unexpected token\n' >&2
		return 2
	fi
	pcm_real "$@"
}
