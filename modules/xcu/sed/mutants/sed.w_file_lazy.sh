# a w file that received nothing is not left behind
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	command sed "$@"
	pcm_rc=$?
	for pcm_a
	do
		for pcm_f in $(printf '%s\n' "$pcm_a" | command sed -n -E 's/^.*(^|[[:blank:]\/])w[[:blank:]]+([^[:blank:]]+)$/\2/p')
		do
			if [ -f "$pcm_f" ] && [ ! -s "$pcm_f" ]; then rm -f "$pcm_f"; fi
		done
	done
	return $pcm_rc
}
