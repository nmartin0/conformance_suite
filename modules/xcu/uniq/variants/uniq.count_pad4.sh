# a conforming uniq whose -c lines are padded to four columns
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the output format gives the count and one blank, and the
# amount of blank space written before the count is not fixed (XCU uniq
# OUTPUT FILES; question Q-175).
uniq() {
	pc_vc=
	for pc_va; do case $pc_va in -*c*) pc_vc=1 ;; esac; done
	if [ -z "$pc_vc" ]; then command uniq "$@"; return; fi
	command uniq "$@" | awk '{ if (match($0, /^ *[0-9]+ /)) { n = substr($0, 1, RLENGTH); sub(/ /, "", n); gsub(/ /, "", n); printf "%4d %s\n", n+0, substr($0, RLENGTH + 1) } else print }'
}
