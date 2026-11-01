# an escaped delimiter inside s is not recognized
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		s?*)
			pcm_d=${pcm_a#s}
			pcm_d=${pcm_d%"${pcm_d#?}"}
			case $pcm_d in
			/) ;;
			*)
				case $pcm_a in
				*'\'"$pcm_d"*) echo "sed: unterminated s command" >&2; return 1 ;;
				esac
				;;
			esac
			;;
		esac
	done
	command sed "$@"
}
