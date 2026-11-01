# more than one blank before the file name of r is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		if printf '%s\n' "$pcm_a" | grep -q '^[0-9$,! {]*r[[:blank:]][[:blank:]]'; then echo "sed: missing filename in r/R/w/W commands" >&2; return 1; fi
	done
	command sed "$@"
}
