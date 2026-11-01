# processing stops at a file that cannot be read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_have=0
	pcm_skip=
	pcm_stop=0
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_n=$((pcm_n - 1))
		if [ "$pcm_stop" = 1 ]; then continue; fi
		if [ -n "$pcm_skip" ]; then pcm_skip=; set -- "$@" "$pcm_a"; continue; fi
		case $pcm_a in
		-e | -f) pcm_have=1; pcm_skip=1; set -- "$@" "$pcm_a" ;;
		-*) set -- "$@" "$pcm_a" ;;
		*)
			if [ "$pcm_have" = 0 ]; then
				pcm_have=1
			elif [ ! -e "$pcm_a" ]; then
				pcm_stop=1
			fi
			set -- "$@" "$pcm_a"
			;;
		esac
	done
	command sed "$@"
}
