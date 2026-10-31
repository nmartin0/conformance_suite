# only the first file operand is read
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_seen=0
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_n=$((pcm_n - 1))
		case $pcm_a in
		-e | -f) set -- "$@" "$pcm_a" "$1"; shift; pcm_n=$((pcm_n - 1)) ;;
		-*) set -- "$@" "$pcm_a" ;;
		*)
			if [ -f "$pcm_a" ]; then
				if [ "$pcm_seen" = 0 ]; then pcm_seen=1; set -- "$@" "$pcm_a"; fi
			else
				set -- "$@" "$pcm_a"
			fi
			;;
		esac
	done
	command sed "$@"
}
