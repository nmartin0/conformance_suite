# #n on any line of the script suppresses the default output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		if printf '%s\n' "$pcm_a" | grep -q '^#n$'; then command sed -n "$@"; return; fi
	done
	command sed "$@"
}
