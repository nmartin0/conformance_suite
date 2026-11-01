# command substitutes an alias for its command name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The shell function stands in for the utility; helpers of the test itself
# (brackets, printf) pass through to the real one.
pcm_real() { ( unset -f command; command "$@" ); }
pcm_alval() {
	pcm_av=$(alias "$1" 2>/dev/null) || return 1
	pcm_av=${pcm_av#*=}
	for pcm_q in "'" '"'; do pcm_av=${pcm_av#"$pcm_q"}; pcm_av=${pcm_av%"$pcm_q"}; done
	return 0
}
command() {
	case $1 in '[' | printf) pcm_real "$@"; return ;; esac
	case $1 in
	-*) pcm_real "$@"; return ;;
	esac
	if pcm_alval "$1"; then
		shift
		eval "$pcm_av"' "$@"'
		return
	fi
	pcm_real "$@"
}
