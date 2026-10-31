# the -e and -f pieces are joined in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_t=pc-mut-s.$$
	: >"$pcm_t"
	pcm_h=0
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		pcm_n=$((pcm_n - 1))
		case $pcm_a in
		-e)
			pcm_h=1
			{ printf '%s\n' "$1"; cat "$pcm_t"; } >"$pcm_t.n"
			mv "$pcm_t.n" "$pcm_t"
			shift
			pcm_n=$((pcm_n - 1))
			;;
		-f)
			pcm_h=1
			{ cat "$1"; printf '\n'; cat "$pcm_t"; } >"$pcm_t.n"
			mv "$pcm_t.n" "$pcm_t"
			shift
			pcm_n=$((pcm_n - 1))
			;;
		*) set -- "$@" "$pcm_a" ;;
		esac
	done
	if [ "$pcm_h" = 1 ]; then command sed -f "$pcm_t" "$@"; else command sed "$@"; fi
	pcm_rc=$?
	rm -f "$pcm_t"
	return $pcm_rc
}
