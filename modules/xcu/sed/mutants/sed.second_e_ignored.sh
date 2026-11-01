# only the first -e option is used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_seen=0
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_n=$((pcm_n - 1))
		case $pcm_a in
		-e)
			if [ "$pcm_seen" = 0 ]; then set -- "$@" -e "$1"; fi
			pcm_seen=1
			shift
			pcm_n=$((pcm_n - 1))
			;;
		*) set -- "$@" "$pcm_a" ;;
		esac
	done
	command sed "$@"
}
