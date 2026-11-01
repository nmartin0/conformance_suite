# the last line written is not followed by a newline
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	command sed "$@" >pc-mut.$$
	pcm_rc=$?
	awk 'NR > 1 { printf "\n" } { printf "%s", $0 }' pc-mut.$$
	rm -f pc-mut.$$
	return $pcm_rc
}
