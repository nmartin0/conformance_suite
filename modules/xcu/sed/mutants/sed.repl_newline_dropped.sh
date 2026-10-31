# a backslash-newline in a replacement is dropped, so the line is not split
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_nl='
'
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		case $pcm_a in
		*"\\$pcm_nl"*) pcm_a=${pcm_a%%"\\$pcm_nl"*}${pcm_a#*"\\$pcm_nl"} ;;
		esac
		set -- "$@" "$pcm_a"
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
}
