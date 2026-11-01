# command -V for an alias does not say alias
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
pcm_alval() {
	pcm_av=$(alias "$1" 2>/dev/null) || return 1
	pcm_av=${pcm_av#*=}
	for pcm_q in "'" '"'; do pcm_av=${pcm_av#"$pcm_q"}; pcm_av=${pcm_av%"$pcm_q"}; done
	return 0
}
pcm_cat() {
	# the kind of name $1, found from the real output $2
	if pcm_alval "$1"; then pcm_kind=alias; return; fi
	if pcm_isspecial "$1" || pcm_isreserved "$1"; then pcm_kind=other; return; fi
	case $2 in
	*unction*) pcm_kind=function ;;
	*/*) pcm_kind=utility ;;
	*) pcm_kind=other ;;
	esac
}
command() {
	case $1 in '[' | printf) pcm_real "$@"; return ;; esac
	case $1 in
	-V)
		pcm_out=$(pcm_real "$@"); pcm_rc=$?
		if [ "$pcm_rc" -eq 0 ] && { pcm_cat "$2" "$pcm_out"; [ "$pcm_kind" = alias ]; }; then
			printf '%s is a macro for %s\n' "$2" "$pcm_av"
			return $pcm_rc
		fi
		if [ -n "$pcm_out" ]; then printf '%s\n' "$pcm_out"; fi
		return $pcm_rc ;;
	esac
	pcm_real "$@"
}
