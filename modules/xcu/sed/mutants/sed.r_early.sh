# r takes the contents of its file when the script starts, not when the output is written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_i=0
	pcm_n=$#
	while [ "$pcm_n" -gt 0 ]; do
		pcm_a=$1
		shift
		for pcm_f in $(printf '%s\n' "$pcm_a" | command sed -n -E 's/^[0-9$,! {]*r[[:blank:]]+([^[:blank:]]+)$/\1/p')
		do
			pcm_i=$((pcm_i + 1))
			if [ -f "$pcm_f" ]; then cp "$pcm_f" "pc-mut-snap.$$.$pcm_i"; else : >"pc-mut-snap.$$.$pcm_i"; fi
			pcm_a=$(printf '%s\n' "$pcm_a" | command sed -E "s/^([0-9\$,! {]*r[[:blank:]]+)$pcm_f\$/\1pc-mut-snap.$$.$pcm_i/")
		done
		set -- "$@" "$pcm_a"
		pcm_n=$((pcm_n - 1))
	done
	command sed "$@"
	pcm_rc=$?
	rm -f pc-mut-snap.$$.*
	return $pcm_rc
}
