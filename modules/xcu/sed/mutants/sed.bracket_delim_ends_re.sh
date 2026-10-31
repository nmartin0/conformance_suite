# the delimiter ends the RE even inside a bracket expression
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		s?*)
			pcm_d=${pcm_a#s}
			pcm_d=${pcm_d%"${pcm_d#?}"}
			case $pcm_a in
			*"[$pcm_d"*) echo "sed: unterminated s command" >&2; return 1 ;;
			esac
			;;
		esac
	done
	command sed "$@"
}
