# r of a file that cannot be read is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		for pcm_f in $(printf '%s\n' "$pcm_a" | command sed -n -E 's/^[0-9$,! {]*r[[:blank:]]+([^[:blank:]]+)$/\1/p')
		do
			if [ ! -r "$pcm_f" ]; then echo "sed: cannot read $pcm_f" >&2; return 2; fi
		done
	done
	command sed "$@"
}
