# the pattern space holds 4096 bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	command sed "$@" >pc-mut.$$
	pcm_rc=$?
	awk '{ print substr($0, 1, 4096) }' pc-mut.$$
	rm -f pc-mut.$$
	return $pcm_rc
}
