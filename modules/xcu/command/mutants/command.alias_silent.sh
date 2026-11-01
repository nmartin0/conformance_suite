# command -v for an alias writes nothing and fails
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
	-v)
		pcm_out=$(pcm_real "$@"); pcm_rc=$?
		if [ "$pcm_rc" -eq 0 ] && pcm_alval "$2"; then
			pcm_rc=1
			return $pcm_rc
		fi
		if [ -n "$pcm_out" ]; then printf '%s\n' "$pcm_out"; fi
		return $pcm_rc ;;
	esac
	pcm_real "$@"
}
