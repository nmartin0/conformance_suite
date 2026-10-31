# the hold space starts with some text instead of empty
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pcm_nl='
'
pcm_pre() {
	pcm_t=$1
	shift
	for pcm_a
	do
		case $pcm_a in
		-e | -f | -[nE]e | -[nE]f) command sed -e "$pcm_t" "$@"; return ;;
		esac
	done
	pcm_n=$#
	pcm_d=0
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		if [ "$pcm_d" = 0 ]; then
			case $pcm_a in
			-*) ;;
			*) pcm_a=$pcm_t$pcm_nl$pcm_a; pcm_d=1 ;;
			esac
		fi
		set -- "$@" "$pcm_a"
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
}
sed() { pcm_pre '1{x;s/^$/Z/;x;}' "$@"; }
