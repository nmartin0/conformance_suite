# l writes a tab as \011
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_l=0
	for pcm_a
	do
		case $pcm_a in l) pcm_l=1 ;; esac
	done
	if [ "$pcm_l" = 0 ]; then command sed "$@"; return; fi
	command sed "$@" >pc-mut.$$
	pcm_rc=$?
	command sed 's/\\t/\\011/g' pc-mut.$$
	rm -f pc-mut.$$
	return $pcm_rc
}
