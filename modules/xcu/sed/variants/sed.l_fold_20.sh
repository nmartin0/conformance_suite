# a conforming sed whose l command folds lines after 20 bytes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the page leaves the length at which l folds unspecified (l).
sed() {
	pcm_l=0
	for pcm_a
	do
		case $pcm_a in l) pcm_l=1 ;; esac
	done
	if [ "$pcm_l" = 0 ]; then command sed "$@"; return; fi
	command sed "$@" >pc-var.$$
	pcm_rc=$?
	awk -v W=20 '
	function flush(   i, c, n, w, t, out) {
		# split the logical line into units (an escape sequence is one unit)
		w = 0; out = ""
		while (length(buf) > 0) {
			c = substr(buf, 1, 1)
			if (c == "\\") {
				if (substr(buf, 2, 1) ~ /[0-7]/) n = 4; else n = 2
			} else n = 1
			t = substr(buf, 1, n); buf = substr(buf, n + 1)
			if (w + n > W - 1 && length(buf) + n > 0 && w > 0) { out = out "\\\n"; w = 0 }
			out = out t; w += n
		}
		printf "%s\n", out
	}
	{
		if (substr($0, length($0), 1) == "\\") { buf = buf substr($0, 1, length($0) - 1); next }
		buf = buf $0; flush(); buf = ""
	}' pc-var.$$
	rm -f pc-var.$$
	return $pcm_rc
}
