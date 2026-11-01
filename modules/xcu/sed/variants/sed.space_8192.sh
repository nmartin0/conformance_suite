# a conforming sed whose pattern space holds 8192 bytes and gives an error beyond that
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the page asks for at least 8192 bytes and says nothing of larger sizes (EXTENDED DESCRIPTION).
sed() {
	command sed "$@" >pc-var.$$
	pcm_rc=$?
	if awk 'length($0) > 8192 { bad = 1 } END { exit bad }' pc-var.$$; then
		cat pc-var.$$
	else
		echo "sed: pattern space overflow" >&2
		pcm_rc=1
	fi
	rm -f pc-var.$$
	return $pcm_rc
}
